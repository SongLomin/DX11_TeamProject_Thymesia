#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_MaskTexture;
texture2D	g_NormalTexture;
texture2D   g_SpecularTexture;
texture2D	g_DissolveTexture;
texture2D	g_DissolveDiffTexture;

vector      g_vCamPosition;
vector      g_vCamLook;
vector      g_vPlayerPosition;
float       g_fMaskingRange = 2.f;

float4	g_vLightFlag;
float   g_fFar = 300.f;
float   g_fDissolveRatio;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;

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

	
	matrix			WorldMatrix= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
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
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector		vLightFlag : SV_Target3;
    vector vORM : SV_Target4;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
	Out.vLightFlag = g_vLightFlag;

	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}

////Normal Mapping ///////////
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
	VS_OUT_NORMAL Out = (VS_OUT_NORMAL)0;


	matrix			WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix)).xyz;
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);
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
	PS_OUT Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	/* 0 ~ 1 */
	float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

	/* -1 ~ 1 */
	vPixelNormal = vPixelNormal * 2.f - 1.f;

	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vPixelNormal = mul(vPixelNormal, WorldMatrix);

	Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
	Out.vLightFlag = g_vLightFlag;
    Out.vORM = 0;
	if (Out.vDiffuse.a < 0.1f)
		discard;

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
	if (fCamToPixelWorld / fCamToPlayer > vMaskTexture.r)
		Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	else
		discard;

	/* 0 ~ 1 */
	float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

	/* -1 ~ 1 */
	vPixelNormal = vPixelNormal * 2.f - 1.f;

	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vPixelNormal = mul(vPixelNormal, WorldMatrix);

	Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
	Out.vLightFlag = g_vLightFlag;

	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}

struct VS_OUT_SHADOW
{
	float4 vPosition : SV_POSITION;
	float4 vProjPos : TEXCOORD0;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
	VS_OUT_SHADOW		Out = (VS_OUT_SHADOW)0;


	matrix			WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
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
	PS_OUT_SHADOW Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.w / g_fFar;

	Out.vLightDepth.a = 1.f;

	return Out;
}

PS_OUT PS_MAIN_RED(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = vector(1.f, 0.f, 0.f, 1.f);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vLightFlag = g_vLightFlag;

    if (Out.vDiffuse.a < 0.1f)
        discard;

    return Out;
}

struct PS_OUT_PBR
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector		vLightFlag : SV_Target3;
	vector      vORM : SV_Target4;
};

PS_OUT_PBR PS_MAIN_NORMAL_PBR(PS_IN_NORMAL In)
{
	PS_OUT_PBR Out = (PS_OUT_PBR)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	/* 0 ~ 1 */
	float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;

	/* -1 ~ 1 */
	vPixelNormal = vPixelNormal * 2.f - 1.f;

	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, float3(In.vNormal.xyz));

	vPixelNormal = mul(vPixelNormal, WorldMatrix);

	Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
	Out.vLightFlag = g_vLightFlag;

	Out.vORM = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	// Out.vORM = g_ORMTexture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_Dissove(PS_IN_NORMAL In)
{
	PS_OUT Out = (PS_OUT)0;

	vector TexDissolve = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV);

	if (g_fDissolveRatio >= TexDissolve.r)
		discard;

	vector vTexDiff = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (g_fDissolveRatio + 0.05f >= TexDissolve.r)
	{
		vTexDiff = g_DissolveDiffTexture.Sample(DefaultSampler, In.vTexUV);
	}
	else
	{
		vTexDiff = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	}

	float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;
	vPixelNormal = vPixelNormal * 2.f - 1.f;

	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vPixelNormal = mul(vPixelNormal, WorldMatrix);

	Out.vDiffuse   = vTexDiff;
	Out.vNormal    = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth     = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
	Out.vLightFlag = g_vLightFlag;

	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass NormalMapping
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

		VertexShader	= compile vs_5_0 VS_MAIN_NORMAL();
		GeometryShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_NORMAL();
	}

	pass ShadowDepth //2
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}

    pass Just_Red
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RED();
    }

	pass Pass4_NonCulling
	{
		SetBlendState			(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState	(DSS_Default, 0);
		SetRasterizerState		(RS_NonCulling);

		VertexShader	= compile vs_5_0	VS_MAIN();
		GeometryShader	= NULL;
		PixelShader		= compile ps_5_0	PS_MAIN();
	}

	pass Pass5_WireFrame_Norm
	{
		SetBlendState			(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState	(DSS_Default, 0);
		SetRasterizerState		(RS_NonCulling);

		VertexShader	= compile vs_5_0	VS_MAIN_NORMAL();
		GeometryShader	= NULL;
		PixelShader		= compile ps_5_0	PS_MAIN_NORMAL();
	}
	pass NoneCulling_Masking//6
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL_MASKING();
	}

    pass Culling_Masking //7
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL_MASKING();
    }

	pass Default_Normal_PBR//8
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_NonCulling);

		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL_PBR();
	}

    pass Pass9_Dissolve //9
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Dissove();
    }
}