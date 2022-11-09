
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_MaskTexture;
texture2D	g_DepthTexture;

float g_fAhlpaScale;
float g_MaskAhlpaScale;
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

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

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

    //Out.vColor.a = 1.f;
    Out.vColor.a = Out.vColor.a * g_fAhlpaScale;
	
	/*if (Out.vColor.a < 0.1f)
	discard;*/

	return Out;
}

struct VS_OUT_SOFT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

VS_OUT_SOFT VS_MAIN_SOFT(VS_IN In)
{
	VS_OUT_SOFT		Out = (VS_OUT_SOFT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN_SOFT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

PS_OUT PS_MAIN_SOFT(PS_IN_SOFT In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float2		vTexUV;

	/* -1, 1 => 1, -1 : 투영공간상의 위치. */
	vTexUV.x = In.vProjPos.x / In.vProjPos.w;
	vTexUV.y = In.vProjPos.y / In.vProjPos.w;

	/* 0, 0 => 1, 1 : 텍스쳐 유브이 좌표. */
	vTexUV.x = vTexUV.x * 0.5f + 0.5f;
	vTexUV.y = vTexUV.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);

	float		fViewZ = vDepthDesc.y * 300.f;

	Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w);

	return Out;

}

PS_OUT PS_MAIN_ADD_MASK(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    float4 MaskColor = g_MaskTexture.Sample(DefaultSampler, In.vTexUV + g_MaskUV);
	
    Out.vColor += MaskColor.a;
	
    Out.vColor.a = Out.vColor.a * g_MaskAhlpaScale;
	
	/*if (Out.vColor.a < 0.1f)
	discard;*/

    return Out;
}


PS_OUT PS_MAIN_CUSTOMUI(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//Out.vColor.a = 1.f;
	//Out.vColor.a = Out.vColor.a * g_fAhlpaScale;

	/*if (Out.vColor.a < 0.1f)
	discard;*/

	return Out;
}

PS_OUT PS_MAIN_CUSTOMUI_FADE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.a *= g_vColor.a;

	//if (Out.vColor.a < 0.1)
	//	discard;

	//Out.vColor.a = 1.f;
	//Out.vColor.a = Out.vColor.a * g_fAhlpaScale;

	/*if (Out.vColor.a < 0.1f)
	discard;*/

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
	pass SoftEffect
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);
		
		VertexShader = compile vs_5_0 VS_MAIN_SOFT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SOFT();
	}
	
	//2
    pass Backgorund
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

	//3
    pass AddMask
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ADD_MASK();
    }

	//4
	pass CustomUI
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CUSTOMUI();
	}
	//5
	pass CustomUI_Font_BG_Alpha
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CUSTOMUI_FADE();
	}

	/* pass Default
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}*/
}