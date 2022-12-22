
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4 g_vShaderFlag;
texture2D	g_DiffuseTexture;
texture2D	g_NormalTexture;
texture2D g_DissolveTexture;
texture2D   g_SpecularTexture;

float g_fFar = 300.f;
//directional dissolve
float3 g_vDissolveStartPos;
float3 g_vDissolveStartEnd;

float3 g_vDissolveDir;
float g_fDissolveAmount;

float4 g_vDiffuse;

bool g_bBloom;
bool g_bGlow;
float4 g_vGlowColor;

float4  g_vCamDir;
float   g_fAhlpa;
float   g_fGlowScale;

struct		tagBoneMatrix
{
	matrix		BoneMatrices[256];
};

tagBoneMatrix		g_Bones;


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float			fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix			BoneMatrix = g_Bones.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_Bones.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_Bones.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_Bones.BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    vector vDepth : SV_TARGET2;
    vector vShaderFlag : SV_Target3;
    vector vORM : SV_Target4;
    vector vExtractBloom : SV_Target5;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    float DissolveDesc = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV).r;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    clip(Out.vDiffuse.a - 0.1f);

    clip(DissolveDesc - g_fDissolveAmount);

    Out.vDiffuse.rgb += float3(1.f, 1.f, 1.f) * step(DissolveDesc - g_fDissolveAmount, 0.02f);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;


    Out.vDiffuse.a = 1.f;


    Out.vExtractBloom = 0;

	return Out;
}

//Shadow

struct VS_IN_SHADOW
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;
    uint4 vBlendIndex : BLENDINDEX;
    float4 vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN_SHADOW In)
{
    VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

    matrix BoneMatrix = g_Bones.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_Bones.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_Bones.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_Bones.BoneMatrices[In.vBlendIndex.w] * fWeightW;


    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct PS_OUT_SHADOW
{
    vector vLightDepth : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
    PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0;

    float DissolveDesc = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV).r;

    clip(DissolveDesc - g_fDissolveAmount);

    Out.vLightDepth.r = In.vProjPos.w / g_fFar;

    Out.vLightDepth.a = 1.f;

    return Out;
}


struct PS_OUT_NONE_DEFERRED
{
    vector vColor : SV_TARGET0;
    vector vExtractBloom : SV_Target1;
    vector vExtractGlow : SV_Target2;
};

PS_OUT_NONE_DEFERRED PS_MAIN_NONE_DEFERRED_USE_AHLPABLEND(PS_IN In)
{
    PS_OUT_NONE_DEFERRED Out = (PS_OUT_NONE_DEFERRED) 0;

    Out.vColor = g_vDiffuse;

    //if (Out.vDiffuse.a < 0.1f)
    //    discard;

    return Out;
}

PS_OUT_NONE_DEFERRED PS_MAIN_NONE_DEFERRED_RIM_LIGHT(PS_IN In)
{
    PS_OUT_NONE_DEFERRED Out = (PS_OUT_NONE_DEFERRED) 0;

    float fDot = dot(In.vNormal, g_vCamDir);
    fDot = abs(fDot);

    Out.vColor = 0.f;

    Out.vColor.a = (1.f - fDot);

    Out.vColor.a *= g_fAhlpa;

    Out.vExtractGlow = float4(0.f, 0.f, 0.f, g_fGlowScale * Out.vColor.a);

    //Out.vColor = g_vDiffuse;


    //if (Out.vDiffuse.a < 0.1f)
    //    discard;

    return Out;
}

struct VS_OUT_NORMAL
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vLocalPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

VS_OUT_NORMAL VS_MAIN_NORMAL(VS_IN In)
{
    VS_OUT_NORMAL Out = (VS_OUT_NORMAL) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

    matrix BoneMatrix = g_Bones.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_Bones.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_Bones.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_Bones.BoneMatrices[In.vBlendIndex.w] * fWeightW;

    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
    vector vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
    Out.vLocalPos = vector(In.vPosition.xyz, 1.f);
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;
    Out.vTangent = normalize(mul(vTangent,g_WorldMatrix)).xyz;
    //Out.vTangent = vTangent.xyz;
    Out.vBinormal = normalize(cross(float3(Out.vNormal.xyz), Out.vTangent));

    return Out;
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN_NORMAL
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vLocalPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};


PS_OUT PS_MAIN_NORMAL(PS_IN_NORMAL In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vTexDiff;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    clip(Out.vDiffuse.a - 0.1f);

    Out.vShaderFlag = g_vShaderFlag;

    /* 0 ~ 1 */
    float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

	/* -1 ~ 1 */
    vPixelNormal = vPixelNormal * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, float3(In.vNormal.xyz));

    vPixelNormal = mul(vPixelNormal, WorldMatrix);

    Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);

    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vORM = 0;

    Out.vDiffuse.a = 1.f;
    Out.vExtractBloom = 0;

    return Out;
}

float IsIn_Range(float fMin, float fMax, float fValue)
{
    return (fMin <= fValue) && (fMax >= fValue);
}

PS_OUT PS_MAIN_DISSOLVE(PS_IN_NORMAL In)
{
    PS_OUT Out = (PS_OUT) 0;

     // normal dissolve
    float DissolveDesc = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV).r;

    clip(DissolveDesc - g_fDissolveAmount);

    float diff = DissolveDesc - g_fDissolveAmount;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vShaderFlag = g_vShaderFlag;

    float fStepValue1 = IsIn_Range(0.03f, 0.05f, diff);
    float fStepValue2 = IsIn_Range(0.05f, 0.065f, diff);
    float fStepValue3 = IsIn_Range(0.065f, 0.08f, diff);

    Out.vDiffuse = IsIn_Range(0.f, 0.03f, diff) * vector(1.f, 0.95f, 0.9f, 1.f) +
                   fStepValue1 * vector(1.f, 0.9f, 0.4f, 1.f) +
                   fStepValue2 * vector(0.9f, 0.1f, 0.f, 1.f) +
                   fStepValue3 * vector(0.3f, 0.0f, 0.f, 1.f) +
                   IsIn_Range(0.08f, 1.f, diff) * g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vShaderFlag = saturate(fStepValue1 + fStepValue2 + fStepValue3) * vector(0.f, 0.f, 1.f, 0.f);
    Out.vExtractBloom = saturate(fStepValue1 + fStepValue2 + fStepValue3) * Out.vDiffuse;
    
    clip(Out.vDiffuse.a - 0.1f);

    float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

    /* -1 ~ 1 */
    vPixelNormal = vPixelNormal * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, float3(In.vNormal.xyz));

    vPixelNormal = mul(vPixelNormal, WorldMatrix);

    Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);

    Out.vORM = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);

    return Out;
}


PS_OUT PS_MAIN_NORMAL_SPECULAR(PS_IN_NORMAL In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    clip(Out.vDiffuse.a - 0.1f);

    Out.vShaderFlag = g_vShaderFlag;

    float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

/* -1 ~ 1 */
    vPixelNormal = vPixelNormal * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, float3(In.vNormal.xyz));

    vPixelNormal = mul(vPixelNormal, WorldMatrix);

    Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);

    Out.vORM = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vExtractBloom = 0;

    return Out;
}

PS_OUT PS_MAIN_NORMAL_DIRECTIONAL_DISSOLVE(PS_IN_NORMAL In)
{
    PS_OUT Out = (PS_OUT) 0;

    //directional dissolve
    float3 vPixelDir = In.vLocalPos.xyz - g_vDissolveStartPos;
    vPixelDir = normalize(vPixelDir);
    float3 vDissolveDir = normalize(g_vDissolveDir);

    float fDotValue = dot(vPixelDir.xyz, vDissolveDir);
    fDotValue = fDotValue * 0.5f + 0.5f;

    clip(fDotValue - g_fDissolveAmount);

    float fDiff = fDotValue - g_fDissolveAmount;
    float fStepValue = IsIn_Range(0.f, 0.015f, fDiff);

    Out.vDiffuse = fStepValue * vector(0.f, 1.f, 0.408f, 1.f) +
                    IsIn_Range(0.015f, 1.f, fDiff) * g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vExtractBloom = fStepValue * Out.vDiffuse;
    
    clip(Out.vDiffuse.a - 0.1f);

    float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

    /* -1 ~ 1 */
    vPixelNormal = vPixelNormal * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, float3(In.vNormal.xyz));

    vPixelNormal = mul(vPixelNormal, WorldMatrix);

    Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;

    Out.vORM = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);


    return Out;
}


technique11 DefaultTechnique
{
	pass Default
	{
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    pass ShadowDepth
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }

    pass AhlpaBlend //2
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONE_DEFERRED_USE_AHLPABLEND();
    }

    pass RimLight //3
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONE_DEFERRED_RIM_LIGHT();
    }

    pass Default_Normal //4
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
    }

    pass Default_Normal_Specular //5
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL_SPECULAR();
    }

    pass Directional_Dissolve //6
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL_DIRECTIONAL_DISSOLVE();
    }

    pass Normal_Dissolve//7
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();
    }

    pass Default_Normal_Specular_NonCulling //8
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_NonCulling );

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL_SPECULAR();
    }
}