
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4 g_vShaderFlag;
texture2D	g_DiffuseTexture;
texture2D   g_NormalTexture;
texture2D   g_MaskTexture;
texture2D   g_SpecularTexture;
texture2D   g_ORMTexture;

vector      g_vCamPosition;
vector      g_vCamLook;
vector      g_vPlayerPosition;
float       g_fMaskingRange = 2.f;
float       g_fMaskingScalar;


float g_fFar = 300.f;

float g_fUVScale;

float g_fColorScale;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
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

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;	
}



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
	vector		vDiffuse : SV_TARGET0;	
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
    vector      vShaderFlag : SV_Target3;
    vector      vORM : SV_Target4;
    vector      vExtractBloom : SV_Target5;
    vector      vExtractGlow : SV_Target6;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);	
    clip(Out.vDiffuse.a - 0.1f);
    
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;
	
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
};

struct VS_OUT_SHADOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN_SHADOW In)
{
    VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
    vector vLightDepth : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
    PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0;

    Out.vLightDepth.r = In.vProjPos.w / g_fFar;
	
    Out.vLightDepth.a = 1.f;

    return Out;
}

// Normal Mapping

struct VS_OUT_NORMAL
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
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

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix)).xyz;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

    return Out;
}



struct PS_IN_NORMAL
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;

};


PS_OUT PS_MAIN_NORMAL(PS_IN_NORMAL In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    clip(Out.vDiffuse.a - 0.1f);
	/* 0 ~ 1 */
    float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

	/* -1 ~ 1 */
    vPixelNormal = vPixelNormal * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

    vPixelNormal = mul(vPixelNormal, WorldMatrix);

    Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;
    Out.vORM = 0;

    Out.vDiffuse.a = 1.f;
    
    Out.vExtractBloom = 0;

    return Out;
}

PS_OUT PS_MAIN_NORMAL_MASKING(PS_IN_NORMAL In)
{
    PS_OUT Out = (PS_OUT)0;
    //플레이어를 가리는 부분은 도트 처리
    float2 vPixelTexUV;
    vPixelTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vPixelTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

    float fCamToPixelWorld = length(g_vCamPosition - In.vWorldPos);
    float fCamToPlayer = length(g_vCamPosition - g_vPlayerPosition);

    vector vMaskTexture = g_MaskTexture.Sample(DefaultSampler, 8.f * vPixelTexUV);
   
    clip(fCamToPixelWorld / fCamToPlayer - vMaskTexture.r);

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    clip(Out.vDiffuse.a - 0.1f);
    /* 0 ~ 1 */
    float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

    /* -1 ~ 1 */
    vPixelNormal = vPixelNormal * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

    vPixelNormal = mul(vPixelNormal, WorldMatrix);

    Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;

    Out.vDiffuse.a = 1.f;
    Out.vExtractBloom = 0;
    
    return Out;
}

PS_OUT PS_MAIN_NORMAL_MASKING_SCALAR(PS_IN_NORMAL In)
{
    PS_OUT Out = (PS_OUT) 0;
    //플레이어를 가리는 부분은 도트 처리
    float2 vPixelTexUV;
    vPixelTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vPixelTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

    vector vMaskTexture = g_MaskTexture.Sample(DefaultSampler, 8.f * vPixelTexUV);
    clip(g_fMaskingScalar - vMaskTexture.r);

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    clip(Out.vDiffuse.a - 0.1f);
    
    /* 0 ~ 1 */
    float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

    /* -1 ~ 1 */
    vPixelNormal = vPixelNormal * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

    vPixelNormal = mul(vPixelNormal, WorldMatrix);

    Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;

    Out.vDiffuse.a = 1.f;
    
    Out.vExtractBloom = 0;

    return Out;
}

struct PS_OUT_FORWARD
{
    vector vColor : SV_TARGET0;
	
};

PS_OUT_FORWARD PS_MAIN_FORWARD(PS_IN In)
{
    PS_OUT_FORWARD Out = (PS_OUT_FORWARD) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * g_fUVScale);
    Out.vColor.rgb *= 0.4f * g_fColorScale;
    Out.vColor.a = 0.8f;

    return Out;
}

PS_OUT      PS_MAIN_PICK(PS_IN In)
{
    PS_OUT		Out = (PS_OUT)0;

    Out.vDiffuse    = vector(1.f, 0.f, 0.f, 0.5f);
    clip(Out.vDiffuse.a - 0.1f);
    
    Out.vNormal     = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth      = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;


    return Out;
}


PS_OUT PS_MAIN_NORMAL_PBR(PS_IN_NORMAL In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    clip(Out.vDiffuse.a - 0.1f);
    
    /* 0 ~ 1 */
    float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

    /* -1 ~ 1 */
     /* -1 ~ 1 */
    vPixelNormal = vPixelNormal * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

    vPixelNormal = mul(vPixelNormal, WorldMatrix);

    Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;

    Out.vORM = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
   // Out.vORM = g_ORMTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vExtractBloom = 0;

    return Out;
}

PS_OUT PS_MAIN_NORMAL_MASKING_SCALAR_PBR(PS_IN_NORMAL In)
{
    PS_OUT Out = (PS_OUT) 0;

    
    float2 vPixelTexUV;
    vPixelTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vPixelTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

    vector vMaskTexture = g_MaskTexture.Sample(DefaultSampler, 8.f * vPixelTexUV);
    
    clip(g_fMaskingScalar - vMaskTexture.r);
    
    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    clip(Out.vDiffuse.a - 0.1f);
    
    /* 0 ~ 1 */
    float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

    /* -1 ~ 1 */
     /* -1 ~ 1 */
    vPixelNormal = vPixelNormal * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

    vPixelNormal = mul(vPixelNormal, WorldMatrix);

    Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;

    Out.vORM = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
   // Out.vORM = g_ORMTexture.Sample(DefaultSampler, In.vTexUV);

    return Out;
}


technique11 DefaultTechnique
{
	pass Default //0
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

    pass ShadowDepth //1
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }

    pass NoneCulling //2
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Normal //3
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
    }

    // Smoke_Prop
    pass UVLoop //4
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FORWARD();
    }
    
    pass Pass5_Pick//5
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader  = NULL;
        PixelShader     = compile ps_5_0 PS_MAIN_PICK();
    }

    pass Masking//6
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL_MASKING();
    }

    pass Default_Normal_PBR//7
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL_PBR();
    }

    pass Masking_Scalar //8
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL_MASKING_SCALAR();
    }

    pass Masking_Scalar_PBR //9
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL_MASKING_SCALAR_PBR();
    }
}