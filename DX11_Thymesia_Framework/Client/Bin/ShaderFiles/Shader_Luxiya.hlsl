#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_OriginalRenderTexture;
texture2D g_MaskTexture;

float2 g_vUV;
float g_fBloomScale;
float g_fRedRatio;
float4 g_vColor;
float4 g_vGlowColor;
float2 g_vPixel;


struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix WorldMatrix = g_WorldMatrix;
    
    matrix matWV, matWVP;

    matWV = mul(WorldMatrix, g_ViewMatrix);
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
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
    vector vExtractBloom : SV_Target1;
    vector vExtractGlow : SV_Target2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    //Out.vColor = g_MaskTexture.Sample(ClampSampler, In.vTexUV + g_vUV);;
    
    Out.vColor = g_OriginalRenderTexture.Sample(ClampSampler, In.vTexUV + g_vUV);
    Out.vColor.a = 1.f;
    //Out.vColor.rgb *= g_MaskTexture.Sample(ClampSampler, In.vTexUV + g_vUV);
    vector MaskDesc = g_MaskTexture.Sample(ClampSampler, In.vTexUV + g_vUV);
    
    Out.vColor.a *= (MaskDesc.r + MaskDesc.g);
    
    //Out.vExtractGlow = g_vGlowColor.a * MaskDesc.g;
    
    //float3 fBloom = min(1.f, max(0.f, g_fBloomScale - MaskDesc.r));
    
    //fBloom.gb *= g_fRedRatio;
    
    //Out.vColor.rgb += fBloom; 
    //Out.vColor.gb -= fBloom * 0.5f;
    
    if (Out.vColor.a < 0.05f)
        discard;
    
    //vector AdjMaskDesc;
    
    //AdjMaskDesc = g_MaskTexture.Sample(ClampSampler, In.vTexUV - g_vPixel.x);
    //if (AdjMaskDesc.r < 0.05f)
    //{
    //    Out.vExtractGlow = g_vGlowColor;
    //}
    
    //AdjMaskDesc = g_MaskTexture.Sample(ClampSampler, In.vTexUV + g_vPixel.x);
    //if (AdjMaskDesc.r < 0.05f)
    //{
    //    Out.vExtractGlow = g_vGlowColor;
    //}
    
    //AdjMaskDesc = g_MaskTexture.Sample(ClampSampler, In.vTexUV - g_vPixel.y);
    //if (AdjMaskDesc.r < 0.05f)
    //{
    //    Out.vExtractGlow = g_vGlowColor;
    //}
    
    //AdjMaskDesc = g_MaskTexture.Sample(ClampSampler, In.vTexUV + g_vPixel.y);
    //if (AdjMaskDesc.r < 0.05f)
    //{
    //    Out.vExtractGlow = g_vGlowColor;
    //}
    
    //AdjMaskDesc = g_MaskTexture.Sample(ClampSampler, In.vTexUV - g_vPixel);
    //if (AdjMaskDesc.r < 0.05f)
    //{
    //    Out.vExtractGlow = g_vGlowColor;
    //}
    
    //AdjMaskDesc = g_MaskTexture.Sample(ClampSampler, In.vTexUV + g_vPixel);
    //if (AdjMaskDesc.r < 0.05f)
    //{
    //    Out.vExtractGlow = g_vGlowColor;
    //}
    
    
    
    
    //Out.vColor.rgb += min(1.f, max(0.f, g_fBloomScale - MaskDesc.r));
    
    //Out.vColor.rgb += 1.f - min(1.f, max(0.f, (MaskDesc.r * g_fBloomScale)));
    
    

    return Out;
}

PS_OUT PS_MAIN_EFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    //Out.vColor = g_MaskTexture.Sample(ClampSampler, In.vTexUV + g_vUV);;
    
    
    //Out.vColor.rgb *= g_MaskTexture.Sample(ClampSampler, In.vTexUV + g_vUV);
    vector MaskDesc = g_MaskTexture.Sample(ClampSampler, In.vTexUV + g_vUV);
    
    Out.vColor = g_vColor;
    Out.vColor.a *= MaskDesc.g;
    Out.vExtractGlow = g_vGlowColor;
    //Out.vExtractGlow.a *= MaskDesc.g;
    
    //Out.vExtractBloom = g_vColor;
    
    if (Out.vColor.a < 0.05f)
        discard;

    return Out;
}


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

    pass Effect
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT();
    }
}