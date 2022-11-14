
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_MaskTexture;
texture2D	g_DepthTexture;

float g_fAhlpaScale;
float g_MaskAhlpaScale;

float g_fAlphaColor;
float g_Ratio;


float2 g_MaskUV;
float4	g_vColor;




struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;	
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{	
	vector		vColor : SV_TARGET0;	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.a *= g_fAlphaColor;

	return Out;
}

PS_OUT PS_MAIN_CUSTOMUI_LOGO_BG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.rgb *= 7.f;

	return Out;
}

PS_OUT PS_MAIN_TEST_RATIO_WIDTH(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (In.vTexUV.x > g_Ratio)
		discard;

	return Out;
}

PS_OUT PS_MAIN_TEST_RATIO_HEIGHT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (1.f - In.vTexUV.y > g_Ratio)
		discard;

	return Out;
}


technique11 DefaultTechnique
{
	//0
	pass Default
	{
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	//1
	pass CustomUI_Logo_BG
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CUSTOMUI_LOGO_BG();
	}
	pass UI_MOON_HPBarRatio_Test_Width //2
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TEST_RATIO_WIDTH();
	}

	pass UI_MOON_HPBarRatio_Test_Height //3
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TEST_RATIO_HEIGHT();
	}
}