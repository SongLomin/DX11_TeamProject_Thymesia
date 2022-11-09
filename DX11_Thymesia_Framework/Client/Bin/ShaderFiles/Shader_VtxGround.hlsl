
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4	g_vLightFlag;
float   g_fDensity;

texture2D	g_SourDiffTexture;
texture2D	g_DestDiffTexture;
texture2D	g_FilterTexture;
texture2D	g_BrushTexture;
texture2D	g_NormalTexture;

/* ---------------------------------------------------------- */

struct VS_IN
{
	float3		vPosition	: POSITION;
	float3		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition	: SV_POSITION;
	float4		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	float4		vWorldPos	: TEXCOORD1;
	float4		vProjPos	: TEXCOORD2;
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

	vector		vSourDiffuse = g_SourDiffTexture.Sample(DefaultSampler, In.vTexUV * 30.f);

    Out.vDiffuse	= vSourDiffuse;
	Out.vDiffuse.a	= 1.f;
	Out.vNormal		= vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth		= vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vLightFlag	= g_vLightFlag;

	return Out;
}

PS_OUT		PS_MAIN_NORM(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vSourDiffuse = g_SourDiffTexture.Sample(DefaultSampler, In.vTexUV * 30.f);

	float3		vPixelNorm	= g_NormalTexture.Sample(DefaultSampler, In.vTexUV * 30.f).xyz;
	vPixelNorm = vPixelNorm * 2.f - 1.f;
	vPixelNorm = mul(vPixelNorm, In.vWorldPos);

    Out.vDiffuse	= vSourDiffuse;
	Out.vDiffuse.a	= 1.f;
	Out.vNormal		= vector(vPixelNorm.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth		= vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
    Out.vLightFlag	= g_vLightFlag;

	return Out;
}

technique11 DefaultTechnique
{
	pass Ground_Default
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader	= compile vs_5_0	VS_MAIN_DEFAULT();
		GeometryShader	= NULL;
        PixelShader		= compile ps_5_0	PS_MAIN_NORM();
    }

	pass Ground_Norm
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader	= compile vs_5_0	VS_MAIN_DEFAULT();
		GeometryShader	= NULL;
        PixelShader		= compile ps_5_0	PS_MAIN_DEFAULT();
    }
}