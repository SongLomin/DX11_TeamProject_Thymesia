#include "Engine_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DepthTexture;
texture2D g_NormalTexture;
texture2D g_OutLineTexture;
texture2D g_OriginalRenderTexture;

float g_ViewPortWidth = 1600.f;
float g_ViewPortHeight = 900.f;

float g_Divider = 1;

// Laplacian Filter
float mask[9] =
{
    -1, -1, -1,
      -1, 8, -1,
      -1, -1, -1
};

// 근처 픽셀
float coord[3] = 
	{ -1, 0, +1 };

float Blur_mask[9] =
{
    1, 1, 1,
      1, 1, 1,
      1, 1, 1
};



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
	
    float4 Color = 0;
    float4 Ret;
   
    for (uint i = 0; i < 9; i++)
    {
        Color += mask[i] * 
		g_DepthTexture.Sample(DefaultSampler,
		In.vTexUV + float2(coord[i % 3] / (g_ViewPortWidth), coord[i / 3] / (g_ViewPortHeight)));
        
        Color += mask[i] *
		g_NormalTexture.Sample(DefaultSampler,
		In.vTexUV + float2(coord[i % 3] / (g_ViewPortWidth), coord[i / 3] / (g_ViewPortHeight))) * 0.1f;

    }
   
    float gray = 1 - (Color.r * 0.3 + Color.g * 0.59 + Color.b * 0.11);
    Ret = float4(gray, gray, gray, 1) / 1;
   
    Out.vColor = smoothstep(0.8, 1, Ret);
	
	
	return Out;
}

PS_OUT PS_MAIN_BLUR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float4 Color = 0;
    float4 Ret;
    
    float2 tex;
    
    for (int i = 0; i < 9; i++)
    {
        tex = In.vTexUV + float2(coord[i % 3] / (g_ViewPortWidth), coord[i / 3] / (g_ViewPortHeight));
        tex = saturate(tex);
        
        Color += Blur_mask[i] * g_OutLineTexture.Sample(DefaultSampler, tex);
    }
    
    Color = Color / 9;
    //Color.a = 1.f;
	
    Out.vColor = Color;
    
    return Out;
}

PS_OUT PS_MAIN_BLEND(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    vector vOutLine = g_OutLineTexture.Sample(DefaultSampler, In.vTexUV);
    vector vOriginal = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV);
	
    Out.vColor = vOutLine * vOriginal;
	
    
	
    return Out;
}

BlendState BS_Default
{
	BlendEnable[0] = false;
};


technique11 DefaultTechnique
{
	pass Default
	{
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    pass Blur_OutLine
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR();
    }

    pass Blend_OriginalRenderTarget
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND();
    }
}