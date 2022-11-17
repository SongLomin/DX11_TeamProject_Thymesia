#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_vCamDirection;

texture2D g_DiffuseTexture;
texture2D g_NoiseTexture;
texture2D g_MaskTexture;

float2 g_vDiffuseUV;
float2 g_vNoiseUV;
float2 g_vMaskUV;

float g_fDiscardRatio;

bool g_bBillboard;

bool g_bBloom;

bool g_bGlow;
float4 g_vGlowColor;

// for Sprite Image
int g_iNumFrameX;
int g_iNumFrameY;

struct VS_IN
{
    float3 vPosition     : POSITION;
    float2 vTexUV        : TEXCOORD0;

    float4 vRight        : TEXCOORD1;
    float4 vUp           : TEXCOORD2;
    float4 vLook         : TEXCOORD3;
    float4 vTranslation  : TEXCOORD4;

    float4 vColor        : TEXCOORD5;
};

struct VS_IN_SPRITE
{
    float3 vPosition     : POSITION;
    float2 vTexUV        : TEXCOORD0;

    float4 vRight        : TEXCOORD1;
    float4 vUp           : TEXCOORD2;
    float4 vLook         : TEXCOORD3;
    float4 vTranslation  : TEXCOORD4;

    float4 vColor        : TEXCOORD5;

    float2 vSpriteUV     : TEXCOORD6;
};

struct VS_OUT
{
    float4 vPosition     : SV_POSITION;
    float2 vTexUV        : TEXCOORD0;
    float4 vColor        : TEXCOORD1;
};

struct VS_OUT_SPRITE
{
    float4 vPosition     : SV_POSITION;
    float2 vTexUV        : TEXCOORD0;
    float4 vColor        : TEXCOORD1;

    float2 vSpriteUV     : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

    if (g_bBillboard)
    {
        float3 vLook  = normalize((g_vCamDirection * -1.f).xyz);
        float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
        float3 vUp    = normalize(cross(vLook, vRight));

        In.vRight     = float4(vRight, 0.f) * length(In.vRight);
        In.vUp        = float4(vUp, 0.f) * length(In.vUp);
        In.vLook      = float4(vLook, 0.f) * length(In.vLook);
    }

    matrix TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

    vector vPosition       = mul(vector(In.vPosition, 1.f), TransformMatrix);

    matrix matWV, matWVP;
    matWV         = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP        = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexUV    = In.vTexUV;
    Out.vColor    = In.vColor;

    return Out;
}

VS_OUT_SPRITE VS_SPRITE(VS_IN_SPRITE In)
{
    VS_OUT_SPRITE Out = (VS_OUT_SPRITE) 0;

    // billboard
    float3 vLook  = normalize((g_vCamDirection * -1.f).xyz);
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
    float3 vUp    = normalize(cross(vLook, vRight));
		
    In.vRight     = float4(vRight, 0.f) * length(In.vRight);
    In.vUp        = float4(vUp, 0.f)    * length(In.vUp);
    In.vLook      = float4(vLook, 0.f)  * length(In.vLook);
    // **billboard
	
    matrix TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    vector vPosition       = mul(vector(In.vPosition, 1.f), TransformMatrix);

    matrix matWV, matWVP;
    matWV         = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP        = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexUV    = In.vTexUV;
    Out.vColor    = In.vColor;

    Out.vSpriteUV = In.vSpriteUV;

    return Out;
}

struct PS_IN
{
    float4 vPosition     : SV_POSITION;
    float2 vTexUV        : TEXCOORD0;
    float4 vColor        : TEXCOORD1;
};

struct PS_IN_SPRITE
{
    float4 vPosition     : SV_POSITION;
    float2 vTexUV        : TEXCOORD0;
    float4 vColor        : TEXCOORD1;

    float2 vSpriteUV     : TEXCOORD2;
};

struct PS_OUT
{
    vector vColor        : SV_TARGET0;
    vector vExtractBloom : SV_Target1;
    vector vExtractGlow  : SV_Target2;
};

PS_OUT PS_MAIN_ALPHADISCARD(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV + g_vDiffuseUV);
    Out.vColor *= In.vColor;

    vector vNoise = g_NoiseTexture.Sample(PointSampler, In.vTexUV + g_vNoiseUV);
    vNoise.rgb = vNoise.rgb * 2 - 1;

    Out.vColor.rgb *= vNoise.rgb;
    Out.vColor.a *= g_MaskTexture.Sample(PointSampler, In.vTexUV + g_vMaskUV).r;

    if (Out.vColor.a < g_fDiscardRatio)
        discard;

    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;

    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}

PS_OUT PS_MAIN_NODIFF_ALPHADISCARD(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    Out.vColor = In.vColor;
    
    vector vNoise = g_NoiseTexture.Sample(PointSampler, In.vTexUV + g_vNoiseUV);
    vNoise.rgb    = vNoise.rgb * 2 - 1;

    Out.vColor.rgb *= vNoise.rgb;
    Out.vColor.a   *= g_MaskTexture.Sample(PointSampler, In.vTexUV + g_vMaskUV).r;

    if (Out.vColor.a < g_fDiscardRatio)
        discard;

    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;

    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}

PS_OUT PS_MAIN_BLACKDISCARD(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV + g_vDiffuseUV);
    Out.vColor *= In.vColor;

    vector vNoise = g_NoiseTexture.Sample(PointSampler, In.vTexUV + g_vNoiseUV);
    vNoise.rgb = vNoise.rgb * 2 - 1;

    Out.vColor.rgb *= vNoise.rgb;
    Out.vColor.a *= g_MaskTexture.Sample(PointSampler, In.vTexUV + g_vMaskUV).r;

    if ((Out.vColor.r + Out.vColor.g + Out.vColor.b) < g_fDiscardRatio)
        discard;

    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;

    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}

PS_OUT PS_MAIN_NODIFF_COLORDISCARD(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    Out.vColor = In.vColor;

    vector vNoise = g_NoiseTexture.Sample(PointSampler, In.vTexUV + g_vNoiseUV);
    vNoise.rgb = vNoise.rgb * 2 - 1;

    Out.vColor.rgb *= vNoise.rgb;
    Out.vColor.a *= g_MaskTexture.Sample(PointSampler, In.vTexUV + g_vMaskUV).r;

    if ((Out.vColor.r + Out.vColor.g + Out.vColor.b) < g_fDiscardRatio)
        discard;

    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;

    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}

PS_OUT PS_SPRITE(PS_IN_SPRITE In)
{
    PS_OUT Out = (PS_OUT) 0;
	
	// Sprite Image
    Out.vColor = g_DiffuseTexture.Sample(PointSampler,
		float2(In.vTexUV.x / g_iNumFrameX + In.vSpriteUV.x,
			In.vTexUV.y / g_iNumFrameY + In.vSpriteUV.y));
	
    Out.vColor *= In.vColor;

    if ((Out.vColor.r + Out.vColor.g + Out.vColor.b) < g_fDiscardRatio)
        discard;
	
    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;
	
    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;
	
    return Out;
}
technique11 DefaultTechnique
{
    pass SpriteImage // 0
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader   = compile vs_5_0 VS_SPRITE();
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_SPRITE();
    }

    pass Default_AlphaDiscard // 1
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALPHADISCARD();
    }

    pass Default_BlackDiscard // 2
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLACKDISCARD();
    }
}