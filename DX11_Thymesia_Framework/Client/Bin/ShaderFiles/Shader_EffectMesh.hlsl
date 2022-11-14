 #include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_vCamDirection;

texture2D g_DiffuseTexture;
texture2D g_NoiseTexture;
texture2D g_MaskTexture;

texture2D g_DepthTexture;
// TODO :  bDynamicNoiseOption temporary for test
bool g_bDynamicNoiseOption;

/**
* Wrap Weight for Textures
*  x : Diff, y : Noise, z : Mask, w : None
*/
bool g_bDiffuseWrap;
bool g_bNoiseWrap;
bool g_bMaskWrap;
vector g_vWrapWeight;

float2 g_vUVDiff;
float2 g_vUVNoise;
float2 g_vUVMask;

float g_fDiscardRatio;

float4 g_vColor;

bool g_bBillboard;

bool g_bBloom;

bool g_bGlow;
float4 g_vGlowColor;

// Vertex Shaders //

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

struct VS_OUT_SOFT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
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
//  Vertex Shaders  //
//  Pixel  Shaders  //
struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct PS_IN_SOFT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
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

PS_OUT PS_MAIN_SOFT(PS_IN_SOFT In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    float2 vTexUV;
    
    vTexUV.x = In.vProjPos.x / In.vProjPos.w;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w;
    
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

PS_OUT PS_DEFAULT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    if (g_bDiffuseWrap)
        Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * g_vWrapWeight.x + g_vUVDiff);
    else
        Out.vColor = g_DiffuseTexture.Sample(ClampSampler, In.vTexUV * g_vWrapWeight.x + g_vUVDiff);
    
    vector vNoise = (vector) 0;
    if (g_bNoiseWrap)
        vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * g_vWrapWeight.y + g_vUVNoise);
    else
        vNoise = g_NoiseTexture.Sample(ClampSampler, In.vTexUV * g_vWrapWeight.y + g_vUVNoise);
    
    vector vMask = (vector) 0;
    if (g_bMaskWrap)
        vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV * g_vWrapWeight.z + g_vUVMask);
    else
        vMask = g_MaskTexture.Sample(ClampSampler, In.vTexUV * g_vWrapWeight.z + g_vUVMask);
    
    // (0, +1) => (-1, +1)
    if (g_bDynamicNoiseOption)
        vNoise.rgb = vNoise.rgb * 2 - 1;
    
    Out.vColor *= g_vColor;
    Out.vColor.rgb *= vNoise.rgb;
    Out.vColor.a *= vMask.r;
    
    if (g_fDiscardRatio > Out.vColor.a)
        discard;
    
    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;
    
    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;
    
    return Out;
}

PS_OUT_DISTORTION PS_DISTORTION(PS_IN In)
{
    PS_OUT_DISTORTION Out = (PS_OUT_DISTORTION) 0;
    
    if (g_bNoiseWrap)
        Out.vColor = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * g_vWrapWeight.y + g_vUVNoise);
    else
        Out.vColor = g_NoiseTexture.Sample(ClampSampler, In.vTexUV * g_vWrapWeight.y + g_vUVNoise);
    
    vector vMask;
    if (g_bMaskWrap)
        vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV * g_vWrapWeight.z + g_vUVMask);
    else
        vMask = g_MaskTexture.Sample(ClampSampler, In.vTexUV * g_vWrapWeight.z + g_vUVMask);
    
    Out.vColor *= g_vColor;
    Out.vColor.a *= vMask.r;
  
    if (Out.vColor.a < g_fDiscardRatio)
        discard;
    
    return Out;
}
//  Pixel  Shaders  //
technique11 DefaultTechnique
{
    pass Default // 0
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);
		
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DEFAULT();
    }

    pass Distortion // 1
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);
		
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DISTORTION();
    }

    pass SoftEffect // 2
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);
		
        VertexShader = compile vs_5_0 VS_MAIN_SOFT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SOFT();
    }
}