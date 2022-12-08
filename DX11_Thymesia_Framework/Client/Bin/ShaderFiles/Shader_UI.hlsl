#include "Client_Shader_Defines.hpp"
matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_MaskTexture;
texture2D	g_DepthTexture;
texture2D	g_DissolveTexture;
texture2D	g_SrcTexture;

float   g_fAhlpaScale;
float   g_MaskAhlpaScale;

float   g_fAlphaColor;
float   g_Ratio;

float2  g_MaskUV;
float4	g_vColor;

float2	g_DissolveStartPt;
float	g_DissolveRange;


float2	g_UVOffset;

int		g_UVIndex;
float2	g_UVAnimMaxSize;
float2 g_NoneAnimRatio = {0.f, 0.f }; //UV가 어디서부터 먹을지 제한해주는 변수.


float	g_fRange;


struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV    : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV    : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV  = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV    = In.vTexUV;

	return Out;	
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV    : TEXCOORD0;
};

struct PS_OUT
{	
	vector vColor    : SV_TARGET0;	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor.a *= g_fAlphaColor;
	return Out;
}

PS_OUT PS_MAIN_CUSTOMUI_LOGO_BG(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor		= g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor.rgb *= 7.f;
	return Out;
}

PS_OUT PS_MAIN_TEST_RATIO_WIDTH(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (In.vTexUV.x > g_Ratio)
		discard;

	return Out;
}

PS_OUT PS_MAIN_TEST_RATIO_HEIGHT(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (1.f - In.vTexUV.y > g_Ratio)
		discard;

	return Out;
}

PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float DissolveDesc    = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV).r + g_Ratio;
	float fDissolveAmount = 1.f - g_Ratio;

	clip(DissolveDesc - fDissolveAmount);
	
	//외곽선 효과 일단 꺼봄
	//Out.vColor.rgb += float3(1.f, 1.f, 1.f) * step(DissolveDesc - fDissolveAmount, 0.02f);
	Out.vColor.a *= max((1.f - In.vTexUV.x) + g_Ratio, 1.f);

	/*
	알파가 1~0으로 가야함.
	*/

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


PS_OUT PS_MAIN_TEST_RATIO_HEIGHT_DISSOLVE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor      = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	float MaskAlpha = g_MaskTexture.Sample(DefaultSampler, In.vTexUV).r;
	
	//제곱 먹여서 알파값이 더욱 극명하게 나오도록
	MaskAlpha = pow(MaskAlpha, 3);

	//진행도 안에 있는 녀석들만 디졸브 먹이고
	if (In.vTexUV.y < g_Ratio)
	{
		float DissolveDesc    = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV).r + g_Ratio;
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

PS_OUT PS_MAIN_WIDTH_DISSOLVE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor      = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	float MaskAlpha = g_MaskTexture.Sample(DefaultSampler, In.vTexUV).r;

	//제곱 먹여서 알파값이 더욱 극명하게 나오도록
	MaskAlpha = pow(MaskAlpha, 3);

	//진행도 안에 있는 녀석들만 디졸브 먹이고
	if (In.vTexUV.x > g_Ratio)
		discard;

	float DissolveDesc    = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV).r + g_Ratio;
	float fDissolveAmount = In.vTexUV.x;
	


	Out.vColor.rgb += float3(0.6f, 0.6f, 0.6f) * step(DissolveDesc - fDissolveAmount, 0.02f);
	
	clip(DissolveDesc - fDissolveAmount);
	
	if (g_Ratio <= 0.0f)
		discard;

	Out.vColor.a *= min(MaskAlpha + g_Ratio, 1.f);//근데 이제 나도 잘 모르겠다.
	return Out;
}


PS_OUT PS_MAIN_WIDTH_DISSOLVE_FACTOR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 DissolveNoise = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV);
	float4 OriginalColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float fFactor = saturate(distance(In.vTexUV, g_DissolveStartPt) + (1.f - g_DissolveRange));
	fFactor = pow(fFactor, 1.f);
	fFactor = 1.f - (fFactor);

	float NewUV = float4(float2(In.vTexUV - DissolveNoise.xy * fFactor * 0.25f), 0.f, fFactor * 10.f).x;
	// WHAT THE FUCK?
	/*float NewUV = float4(float2(In.vTexUV - DissolveNoise * fFactor * 0.25f),
		0.f,
		fFactor * 10.f
	);*/
	//Out.vColor = g_DiffuseTexture.SampleLevel(DefaultSampler, NewUV, 1);

	return Out;
}

PS_OUT PS_MAIN_PARRY(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor        = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float fRatio      = 1.f - g_Ratio;

	float fRatioLeft  = fRatio / 2;
	float fRatioRight = 1.f - (fRatio / 2);

	Out.vColor.g      = 1 - g_Ratio;

	if (In.vTexUV.x < fRatioLeft)
		discard;

	if (In.vTexUV.x > fRatioRight)
		discard;

	return Out;
}


PS_OUT PS_MAIN_BORDER_OUT(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor.a *= g_fAlphaColor;

	if (In.vTexUV.x < 0.1f 
		|| In.vTexUV.x > 0.9f 
		|| In.vTexUV.y < 0.1f 
		|| In.vTexUV.y > 0.9f
		)
		Out.vColor.a *= 0.1f;

	return Out;
}

PS_OUT PS_MASK_UV_ANIM_EVOLVE_LEVELBG(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 destColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float4	SrcColor = g_SrcTexture.Sample(DefaultSampler, In.vTexUV);

    SrcColor.rgba *= 0.4f;
	
    Out.vColor = (destColor * g_fAlphaColor) + SrcColor * (1 - g_fAlphaColor);

	Out.vColor.a *= g_fAlphaColor;

	if (Out.vColor.a < 0.1f)
	{
		discard;
	}
	else
	{
		float	maskMag = 8.f;
		float2	maskUV = In.vTexUV.rg;
		
		float4	mask = g_MaskTexture.Sample(DefaultSampler, maskUV + g_UVOffset);//마스크 생성.
		//마스크의 알파값에 따라 출력할 색상을 처리해줌.
		//얘의 색상을 그대로 쓰는게 아니라, 배율로 처리해줄거임..
	
		/*
			0~1값이 최대 4배가 되어야함.
		
			마스킹 위치가 1이다.->4에 가깝게
		*/
        Out.vColor.rgb = (Out.vColor.rgb * max((mask * maskMag).x, 1.f));
    }

	return Out;
}

PS_OUT PS_MASK_UV_TEST(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	
    float4 diffuseColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	
    float2 MaskUV = In.vTexUV;
	
    int2 MaskIndex;
    int  index = (int)g_Ratio;
	
	
    MaskIndex.x = index % 4; //0~3
    MaskIndex.y = index / 4; //0~1

    float2 MaskOffset;
	
    MaskOffset.x = ((float) MaskIndex.x) / 4.f;
    MaskOffset.y = ((float) MaskIndex.y) / 2.f;
	
	
	/*
	0~0.25
	0.25~0.5



	*/
	

    MaskUV.x = MaskOffset.x + (In.vTexUV.x / 4.f);
    MaskUV.y = MaskOffset.y + (In.vTexUV.y / 2.f);
	
    float4 maskColor = g_MaskTexture.Sample(DefaultSampler, MaskUV);
	
	
    diffuseColor.rgb *= maskColor.rgb;
	
    if (maskColor.r < 0.7f)
    {
        discard;
    }
    Out.vColor = diffuseColor;
    Out.vColor.a *= g_fAlphaColor;
	
	
	return Out;
}

PS_OUT PS_MASK_UV_ANIM(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 destColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
    Out.vColor = destColor;
    
	//해당 비율 범위 내에 있는 애들은 마스킹 무시하고 출력
    if (g_NoneAnimRatio.x > In.vTexUV.x || g_NoneAnimRatio.y > In.vTexUV.y)
    {
        return Out;
    }
	
    float2 maskUV = In.vTexUV.rg;
    
	float4 mask = g_MaskTexture.Sample(DefaultSampler, maskUV + g_UVOffset); //마스크 생성.

    Out.vColor.a *= g_fAlphaColor;
    Out.vColor.a +=  mask.r;
    
    if (Out.vColor.a < 0.1f)
    {
        discard;
    }
    return Out;
}


PS_OUT PS_MASK_UV_DISCARD_RANGE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    float MaskAlpha = g_MaskTexture.Sample(DefaultSampler, In.vTexUV).r;
	
	//제곱 먹여서 알파값이 더욱 극명하게 나오도록
    MaskAlpha = pow(MaskAlpha, 3);

    float DissolveDesc = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV).r + g_Ratio;
    float fDissolveAmount = g_Ratio;
	
    clip(DissolveDesc - fDissolveAmount);

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

		VertexShader   = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN();
	}
	//1
	pass CustomUI_Logo_BG
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader   = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_CUSTOMUI_LOGO_BG();
	}
	pass UI_MOON_HPBarRatio_Test_Width //2
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader   = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_TEST_RATIO_WIDTH();
	}

	pass UI_MOON_HPBarRatio_Test_Height //3
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader   = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_TEST_RATIO_HEIGHT();
	}


	pass UI_Dissolve//4
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader   = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_DISSOLVE();
	}

	pass UI_PlagueWeapon_Steal_Icon//5
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader   = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_TEST_RATIO_HEIGHT_DISSOLVE();
	}

	pass UI_Dissolove_Horizontal_masking//6
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader   = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_WIDTH_DISSOLVE_FACTOR();
	}
	pass UI_Parrying//7
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader   = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_PARRY();
	}
	pass UI_EvolveMenuRightBG//8
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader   = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_BORDER_OUT();
	}

	pass UI_UVAnimaitonEvolveLevel//9
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MASK_UV_ANIM_EVOLVE_LEVELBG();
    }
    pass UI_UVTest//10
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MASK_UV_TEST();
    }
    pass UI_UVAnimaiton//11
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MASK_UV_ANIM();
    }
    pass UI_UV_Discard_Range//12
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MASK_UV_DISCARD_RANGE();
    }
}