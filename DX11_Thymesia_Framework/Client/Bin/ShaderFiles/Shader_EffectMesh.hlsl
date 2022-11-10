#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_vCamDirection;

texture2D g_DiffuseTexture;
texture2D g_DepthTexture;
texture2D g_MaskTexture;
texture2D g_NoiseTexture;

// TODO :  bDynamicNoiseOption temporary for test
bool g_bDynamicNoiseOption;

/**
* Wrap Weight
*  x : Diff, y : Mask, z : Noise, w : None
*/
vector g_vWrapWeight;

// texture2D g_GradientTexture;

float g_fDiscardRatio;

float4 g_vColor;
float2 g_vUV;

bool g_bBloom;

bool g_bGlow;
float4 g_vGlowColor;

bool g_bDiffuseMap;
bool g_bBillboard;

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
    
    if (g_bBillboard)
    {
        float3 vLook = normalize((g_vCamDirection * -1.f).xyz);
        float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
        float3 vUp = normalize(cross(vLook, vRight));
		
        WorldMatrix[0] = float4(vRight, 0.f) * length(WorldMatrix[0]);
        WorldMatrix[1] = float4(vUp, 0.f) * length(WorldMatrix[1]);
        WorldMatrix[2] = float4(vLook, 0.f) * length(WorldMatrix[2]);
    }
    
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

struct PS_OUT_DISTORTION
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    
    Out.vColor = g_vColor;
    
    if (g_bDiffuseMap)
    {
        Out.vColor *= g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    }
    

    Out.vColor.a = Out.vColor.a;
	
    if (g_bBloom)
    {
        Out.vExtractBloom = Out.vColor;
    }
    
    if (g_bGlow)
    {
        Out.vExtractGlow = g_vGlowColor;
    }
    
    if (Out.vColor.a < 0.1f)
        discard;

    return Out;
}

struct VS_OUT_SOFT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

VS_OUT_SOFT VS_MAIN_SOFT(VS_IN In)
{
    VS_OUT_SOFT Out = (VS_OUT_SOFT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN_SOFT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

PS_OUT PS_MAIN_SOFT(PS_IN_SOFT In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    float2 vTexUV;

	/* -1, 1 => 1, -1 : 투영공간상의 위치. */
    vTexUV.x = In.vProjPos.x / In.vProjPos.w;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w;

	/* 0, 0 => 1, 1 : 텍스쳐 유브이 좌표. */
    vTexUV.x = vTexUV.x * 0.5f + 0.5f;
    vTexUV.y = vTexUV.y * -0.5f + 0.5f;

    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);

    float fViewZ = vDepthDesc.y * 300.f;

    Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w);
    
    if (g_bBloom)
    {
        Out.vExtractBloom = Out.vColor;
    }
    if (g_bGlow)
    {
        Out.vExtractGlow = g_vGlowColor;
    }

    return Out;

}

PS_OUT PS_MAIN_NONE_DIFFUSE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_vColor;
    
    if (g_bDiffuseMap)
    {
        Out.vColor *= g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV + g_vUV);
    }
   
    vector MaskColor = g_MaskTexture.Sample(DefaultSampler, In.vTexUV + g_vUV);
    
    Out.vColor.a *= MaskColor.r;
	
    if (Out.vColor.a < g_fDiscardRatio)
        discard;
    
    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;
    
    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}

PS_OUT PS_MAIN_NONE_DIFFUSE_CLAMP(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_vColor;
    
    if (g_bDiffuseMap)
    {
        Out.vColor *= g_DiffuseTexture.Sample(ClampSampler, In.vTexUV + g_vUV);
    }
    
    vector MaskColor = g_MaskTexture.Sample(ClampSampler, In.vTexUV + g_vUV);
    
    Out.vColor.a *= MaskColor.r;
	
    if (Out.vColor.a < g_fDiscardRatio)
        discard;

    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;
    
    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;
    
    return Out;
}

PS_OUT PS_DIFF_MASK_NOISE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_vColor /*g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * g_vWrapWeight.x + g_vUV)*/;
    vector vMaskWeight = g_MaskTexture.Sample(DefaultSampler, In.vTexUV * g_vWrapWeight.y + g_vUV);
    vector vNoiseWeight = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * g_vWrapWeight.z + g_vUV);
    
    // (0, +1) => (-1, +1)
    if (g_bDynamicNoiseOption)
        vNoiseWeight.rgb = vNoiseWeight.rgb * 2 - 1;
    
    // Out.vColor *= g_vColor;
    
    Out.vColor.rgb *= vNoiseWeight.rgb;
   
    Out.vColor.a *= vMaskWeight.r;
    
    if (Out.vColor.a < g_fDiscardRatio)
        discard;
    
    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;
    
    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}
PS_OUT_DISTORTION PS_DISTORTION(PS_IN In)
{
    PS_OUT_DISTORTION Out = (PS_OUT_DISTORTION)0;
    
    Out.vColor = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);

    return Out;
}

technique11 DefaultTechnique
{
    pass Default // 0
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass SoftEffect // 1
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);
		
        VertexShader = compile vs_5_0 VS_MAIN_SOFT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SOFT();
    }
    
    pass NoneDiffuseEffect_UVDefault // 2
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);
		
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONE_DIFFUSE();
    }

    pass NoneDiffuseEffect_UVClamp // 3
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);
		
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONE_DIFFUSE_CLAMP();
    }

    pass PerfectCustom // 4
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);
		
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DIFF_MASK_NOISE();
    }

    pass Distortion //5
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_SOFT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DISTORTION();
    }
}