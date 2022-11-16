
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_MaskTexture;
texture2D	g_DepthTexture;
texture2D	g_DissolveTexture;

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

PS_OUT PS_MAIN_TEST_RATIO_HEIGHT_DISSOLVE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float		MaskAlpha = g_MaskTexture.Sample(DefaultSampler, In.vTexUV).r;
	
	//제곱 먹여서 알파값이 더욱 극명하게 나오도록
	MaskAlpha = pow(MaskAlpha, 3);

	//진행도 안에 있는 녀석들만 디졸브 먹이고
	if (In.vTexUV.y < g_Ratio)
	{
		float DissolveDesc = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV).r + g_Ratio;
		float fDissolveAmount = 0.5f - g_Ratio;
		clip(DissolveDesc - fDissolveAmount);
	}

	Out.vColor.rgba *= min(MaskAlpha + g_Ratio, 1.f);//근데 이제 나도 잘 모르겠다.
	
	//
	
	//	discard;

	//g_fDissolveAmount  = 0~1.f; 0로 가면갈수록 안보임.
	
	/*
	
	float DissolveDesc = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV).r;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    clip(DissolveDesc - g_fDissolveAmount);

    Out.vDiffuse.rgb += float3(1.f, 1.f, 1.f) * step(DissolveDesc - g_fDissolveAmount, 0.02f);
g_fDissolveAmount
0~1
	
	*/
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

	pass UI_PlagueWeapon_Steal_Icon//4
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TEST_RATIO_HEIGHT_DISSOLVE();
	}
}