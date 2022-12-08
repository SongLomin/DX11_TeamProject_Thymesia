
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D   g_OriginalRenderTexture;

texture2D	g_ExtractMapTexture;

float g_PixelWidth;
float g_PixelHeight;

float g_BlurStrength;
  
float PixelKernel[13] =  
{  
    -6,  
    -5,  
    -4,  
    -3,  
    -2,  
    -1,  
     0,  
     1,  
     2,  
     3,  
     4,  
     5,  
     6,  
};  
  
static const float BlurWeights[13] =   
{  
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
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
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;

};

struct PS_OUT
{	
	vector		vBlurTexture : SV_TARGET0;	
};

struct PS_OUT_EXTRACT
{
    vector vOriginal : SV_TARGET0;
    vector vExtractBloom : SV_TARGET1;
    vector vExtractGlow : SV_TARGET2;
};

PS_OUT PS_MAIN_BLURX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	

	// Apply surrounding pixels  
    float4 color = 0;  
    float2 samp = In.vTexUV;  

	//vector vExtractBloom = ;
  
    for (uint i = 0; i < 13; i++) {  
        samp.x = In.vTexUV.x + PixelKernel[i] * g_PixelWidth;
        color += g_ExtractMapTexture.Sample(DefaultSampler, samp) * BlurWeights[i] * g_BlurStrength;
    }  
    
    Out.vBlurTexture = color;

    return Out;
}

PS_OUT PS_MAIN_BLURY(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	// Apply surrounding pixels  
    float4 color = 0;
    float2 samp = In.vTexUV;

	//vector vExtractBloom = ;
  
    for (int i = 0; i < 13; i++)
    {
        samp.y = In.vTexUV.y + PixelKernel[i] * g_PixelHeight;
        color += g_ExtractMapTexture.Sample(DefaultSampler, samp) * BlurWeights[i] * g_BlurStrength;
    }
    
    Out.vBlurTexture = color;

    return Out;
}

PS_OUT_EXTRACT PS_MAIN_EXTRACT_BRIGHTNESS(PS_IN In)
{
    PS_OUT_EXTRACT Out = (PS_OUT_EXTRACT) 0;

    vector vOriginalColor = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV);
    
    //Out.vColor = vOriginalColor;
    //Out.vColor.a = 1.f;
    
    float4 BrightColor = (float4) 0.f;

    float brightness = dot(vOriginalColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    if (brightness > 0.85)
        BrightColor = float4(vOriginalColor.rgb, 1.0);
    else
    {
        discard;
    }
    Out.vExtractBloom = BrightColor;
    
    return Out;
}

PS_OUT PS_MAIN_BLURX_GLOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	
    float4 color = 0;
    float2 samp = In.vTexUV;
    float4 SampleColor = 0;
    uint Index = 0;
  
    for (uint i = 0; i < 13; i++)
    {
        samp.x = In.vTexUV.x + PixelKernel[i] * g_PixelWidth;
        SampleColor = g_ExtractMapTexture.Sample(DefaultSampler, samp);
        
        if (SampleColor.a > 0.03f)
        {
            color.rgb += SampleColor.rgb;
            ++Index;
        }
        
        color.a += SampleColor.a * BlurWeights[i] * g_BlurStrength;
    }
    
    color.rgb /= Index;
    
    Out.vBlurTexture = color;
    
    if(Out.vBlurTexture.a < 0.01f)
    {
        discard;
    }
    
    return Out;
}

PS_OUT PS_MAIN_BLURY_GLOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	float4 color = 0;
	float2 samp = In.vTexUV;
	float4 SampleColor = 0;
	uint Index = 0;

	for (uint i = 0; i < 13; i++)
	{
		samp.y = In.vTexUV.y + PixelKernel[i] * g_PixelHeight;
		SampleColor = g_ExtractMapTexture.Sample(DefaultSampler, samp);

		if (SampleColor.a > 0.03f)
		{
			color.rgb += SampleColor.rgb;
			++Index;
		}

		color.a += SampleColor.a * BlurWeights[i] * g_BlurStrength;
	}

	color.rgb /= Index;

	Out.vBlurTexture = color;

	if (Out.vBlurTexture.a < 0.01f)
	{
		discard;
	}

	return Out;

}

PS_OUT PS_MAIN_BLURX_WITH_ORIGINAL_RENDER_TEXTURE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;


	float4 color = 0;
	float2 samp = In.vTexUV;
	float4 SampleColor = 0;
	uint Index = 0;

	for (uint i = 0; i < 13; i++)
	{
		samp.x = In.vTexUV.x + PixelKernel[i] * g_PixelWidth;
		SampleColor = g_ExtractMapTexture.Sample(DefaultSampler, samp);

		if (SampleColor.a > 0.03f)
		{
			color.rgb += SampleColor.rgb;
			++Index;
		}

		color.a += SampleColor.a * BlurWeights[i] * g_BlurStrength;
	}

	color.rgb /= Index;

	Out.vBlurTexture = color;

	if (Out.vBlurTexture.a < 0.01f)
	{
		discard;
	}

	return Out;
}

PS_OUT PS_MAIN_BLURY_WITH_ORIGINAL_RENDER_TEXTURE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 color = 0;
	float2 samp = In.vTexUV;

	//vector vExtractBloom = ;

	for (int i = 0; i < 13; i++)
	{
		samp.y = In.vTexUV.y + PixelKernel[i] * g_PixelHeight;
		color.a += g_ExtractMapTexture.Sample(DefaultSampler, samp).a * BlurWeights[i] * g_BlurStrength;
	}

	Out.vBlurTexture = color;

	return Out;
}


technique11 DefaultTechnique
{
	pass BlurX //0
	{
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLURX();
	}

    pass BlurY //1
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLURY();
    }

    pass Extract_Brightness //2
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EXTRACT_BRIGHTNESS();
    }

    pass BlurX_Glow //3
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLURX_GLOW();
    }

    pass BlurY_Glow //4
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLURY_GLOW();
    }

	pass BlurX_With_Origin //5
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLURX_WITH_ORIGINAL_RENDER_TEXTURE();
	}

	pass BlurY_With_Origin //6
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLURY_WITH_ORIGINAL_RENDER_TEXTURE();
	}
}