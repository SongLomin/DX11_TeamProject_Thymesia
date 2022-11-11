
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4	g_vLightFlag;
float   g_fDensity;

texture2D	g_Texture_Sorc_Diff	 ,	g_Texture_Sorc_Norm	;  int g_Texture_Sorc_Type;
texture2D	g_Texture_AddNo1_Diff,	g_Texture_AddNo1_Norm; int g_Texture_AddNo1_Type;
texture2D	g_Texture_AddNo2_Diff,	g_Texture_AddNo2_Norm; int g_Texture_AddNo2_Type;
texture2D	g_Texture_AddNo3_Diff,	g_Texture_AddNo3_Norm; int g_Texture_AddNo3_Type;
texture2D	g_FilterTexture;

/* ---------------------------------------------------------- */

struct VS_IN
{
	float3		vPosition	: POSITION;
	float3		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	float3		vTangent	: TANGENT;
};

struct VS_OUT
{
	float4		vPosition	: SV_POSITION;
	float4		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	float4		vWorldPos	: TEXCOORD1;
	float4		vProjPos	: TEXCOORD2;
	float3		vTangent	: TANGENT;
	float3		vBinormal	: BINORMAL;
};

/* ---------------------------------------------------------- */

VS_OUT		VS_MAIN_DEFAULT(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition	= mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV		= In.vTexUV;
	Out.vNormal		= normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos	= mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vProjPos	= Out.vPosition;
	Out.vTangent	= normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal	= normalize(cross(float3(Out.vNormal.xyz), Out.vTangent));

	return Out;
}

/* ---------------------------------------------------------- */

struct PS_IN
{
	float4		vPosition	: SV_POSITION;
	float4		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	float4		vWorldPos	: TEXCOORD1;
	float4		vProjPos	: TEXCOORD2;
	float3		vTangent	: TANGENT;
	float3		vBinormal	: BINORMAL;
};

struct PS_OUT
{
	vector		vDiffuse	: SV_TARGET0;
	vector		vNormal		: SV_TARGET1;
	vector		vDepth		: SV_TARGET2;
    vector		vLightFlag	: SV_Target3;
};

/* ---------------------------------------------------------- */

PS_OUT		PS_MAIN_DEFAULT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    Out.vDiffuse = g_Texture_Sorc_Diff.Sample(DefaultSampler, In.vTexUV * g_fDensity);;

	vector		vFilterDiffuse	= g_FilterTexture.Sample(DefaultSampler, In.vTexUV);

	if (0.f < vFilterDiffuse.r)
		Out.vDiffuse = g_Texture_AddNo1_Diff.Sample(DefaultSampler, In.vTexUV * g_fDensity);

	if (0.f < vFilterDiffuse.g)
		Out.vDiffuse = g_Texture_AddNo2_Diff.Sample(DefaultSampler, In.vTexUV * g_fDensity);

	if (0.f < vFilterDiffuse.b)
		Out.vDiffuse = g_Texture_AddNo3_Diff.Sample(DefaultSampler, In.vTexUV * g_fDensity);

	Out.vDiffuse.a	= 1.f;
	Out.vNormal		= In.vNormal;
	Out.vDepth		= vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vLightFlag	= g_vLightFlag;

	return Out;
}

PS_OUT		PS_MAIN_NORM(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_Texture_Sorc_Diff.Sample(DefaultSampler, In.vTexUV * g_fDensity);

	vector		vFilterDiffuse	= g_FilterTexture.Sample(DefaultSampler, In.vTexUV);
	float3		vPixelNorm		= g_Texture_Sorc_Norm.Sample(DefaultSampler, In.vTexUV * g_fDensity).xyz;

	if (0.f < vFilterDiffuse.r)
	{
		Out.vDiffuse	= g_Texture_AddNo1_Diff.Sample(DefaultSampler, In.vTexUV * g_fDensity);
		vPixelNorm		= g_Texture_AddNo1_Norm.Sample(DefaultSampler, In.vTexUV * g_fDensity).xyz;
	}

	if (0.f < vFilterDiffuse.g)
	{
		Out.vDiffuse	= g_Texture_AddNo2_Diff.Sample(DefaultSampler, In.vTexUV * g_fDensity);
		vPixelNorm		= g_Texture_AddNo2_Norm.Sample(DefaultSampler, In.vTexUV * g_fDensity).xyz;
	}

	if (0.f < vFilterDiffuse.b)
	{
		Out.vDiffuse	= g_Texture_AddNo3_Diff.Sample(DefaultSampler, In.vTexUV * g_fDensity);
		vPixelNorm		= g_Texture_AddNo3_Norm.Sample(DefaultSampler, In.vTexUV * g_fDensity).xyz;
	}

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, float3(In.vNormal.xyz));
	vPixelNorm = vPixelNorm * 2.f - 1.f;
	vPixelNorm = mul(vPixelNorm, WorldMatrix);

	Out.vDiffuse.a	= 1.f;
	Out.vNormal		= vector(vPixelNorm.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth		= vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vLightFlag	= g_vLightFlag;

	return Out;
}

PS_OUT		PS_MAIN_WIREFRAM(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    Out.vDiffuse	= vector(1.f, 1.f, 0.f, 1.f);
	Out.vNormal		= In.vNormal;
	Out.vDepth		= vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vLightFlag	= g_vLightFlag;

	return Out;
}

PS_OUT		PS_MAIN_FILLTER(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    Out.vDiffuse = g_Texture_Sorc_Diff.Sample(DefaultSampler, In.vTexUV * g_fDensity);;

	vector		vFilterDiffuse	= g_FilterTexture.Sample(DefaultSampler, In.vTexUV);

	if (0.1f < vFilterDiffuse.r)
		Out.vDiffuse = vFilterDiffuse;

	if (0.1f < vFilterDiffuse.g)
		Out.vDiffuse = vFilterDiffuse;

	if (0.1f < vFilterDiffuse.b)
		Out.vDiffuse = vFilterDiffuse;

	Out.vDiffuse.a	= 1.f;
	Out.vNormal		= In.vNormal;
	Out.vDepth		= vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vLightFlag	= g_vLightFlag;

	return Out;
}

/* ---------------------------------------------------------- */

technique11 DefaultTechnique
{
	pass PASS_0_Ground_Default
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader	= compile vs_5_0	VS_MAIN_DEFAULT();
		GeometryShader	= NULL;
        PixelShader		= compile ps_5_0	PS_MAIN_DEFAULT();
    }

	pass PASS_1_Ground_Norm
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader	= compile vs_5_0	VS_MAIN_DEFAULT();
		GeometryShader	= NULL;
        PixelShader		= compile ps_5_0	PS_MAIN_NORM();
    }

	pass PASS_2_Ground_WireFrame
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Wireframe);

		VertexShader	= compile vs_5_0	VS_MAIN_DEFAULT();
		GeometryShader	= NULL;
        PixelShader		= compile ps_5_0	PS_MAIN_WIREFRAM();
    }

	pass PASS_3_Ground_OnlyFilterTexture
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader	= compile vs_5_0	VS_MAIN_DEFAULT();
		GeometryShader	= NULL;
		PixelShader		= compile ps_5_0	PS_MAIN_FILLTER();
	}
}