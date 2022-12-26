#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_vCamDirection;

texture2D g_DiffuseTexture;
texture2D g_NoiseTexture;
texture2D g_MaskTexture;

texture2D g_DepthTexture;

float2 g_vDiffuseUV;
float2 g_vNoiseUV;
float2 g_vMaskUV;

float g_fDiscardRatio;

bool g_bBillboard;

bool g_bBloom;

bool g_bGlow;
float4 g_vGlowColor;

// for Sprite Image
bool g_bUseMask;
int g_iNumFrameX;
int g_iNumFrameY;

bool g_bXInverse;
bool g_bYInverse;

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

struct VS_OUT_SOFT
{
    float4 vPosition     : SV_POSITION;
    float2 vTexUV        : TEXCOORD0;
    float4 vColor        : TEXCOORD1;
    float4 vProjPos      : TEXCOORD2;
};

struct VS_OUT_SPRITE
{
    float4 vPosition     : SV_POSITION;
    float2 vTexUV        : TEXCOORD0;
    float4 vColor        : TEXCOORD1;

    float2 vSpriteUV     : TEXCOORD2;
};

struct VS_OUT_SPRITE_SOFT
{
    float4 vPosition     : SV_POSITION;
    float2 vTexUV        : TEXCOORD0;
    float4 vColor        : TEXCOORD1;

    float2 vSpriteUV     : TEXCOORD2;

    float4 vProjPos      : TEXCOORD3;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

    matrix TransformMatrix = mul(float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation), g_WorldMatrix);

    if (g_bBillboard)
    {
        float3 vLook = normalize((g_vCamDirection * -1.f).xyz);
        float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
        float3 vUp = normalize(cross(vLook, vRight));

		TransformMatrix[0] = float4(vRight, 0.f) * length(TransformMatrix[0]);
		TransformMatrix[1] = float4(vUp, 0.f) * length(TransformMatrix[1]);
		TransformMatrix[2] = float4(vLook, 0.f) * length(TransformMatrix[2]);
    }

    vector vPosition = mul(vector(In.vPosition, 1.f),  TransformMatrix);

    matrix matVP;
	matVP = mul(g_ViewMatrix, g_ProjMatrix);
	Out.vPosition = mul(vPosition, matVP);
	Out.vTexUV = In.vTexUV;
	Out.vColor = In.vColor;

    return Out;
}

VS_OUT_SOFT VS_SOFT(VS_IN In)
{
    VS_OUT_SOFT Out = (VS_OUT_SOFT)0;

    matrix TransformMatrix = mul(float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation), g_WorldMatrix);

    if (g_bBillboard)
    {
        float3 vLook = normalize((g_vCamDirection * -1.f).xyz);
        float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
        float3 vUp = normalize(cross(vLook, vRight));

        TransformMatrix[0] = float4(vRight, 0.f) * length(TransformMatrix[0]);
        TransformMatrix[1] = float4(vUp, 0.f) * length(TransformMatrix[1]);
        TransformMatrix[2] = float4(vLook, 0.f) * length(TransformMatrix[2]);
    }

    vector vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

    matrix matVP;
    matVP = mul(g_ViewMatrix, g_ProjMatrix);
    Out.vPosition = mul(vPosition, matVP);
    Out.vTexUV = In.vTexUV;
    Out.vColor = In.vColor;
    Out.vProjPos = Out.vPosition;
    return Out;
}

VS_OUT_SPRITE VS_SPRITE(VS_IN_SPRITE In)
{
    VS_OUT_SPRITE Out = (VS_OUT_SPRITE) 0;

    matrix TransformMatrix = mul(float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation), g_WorldMatrix);

    float3 vLook = normalize((g_vCamDirection * -1.f).xyz);
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
    float3 vUp = normalize(cross(vLook, vRight));

    TransformMatrix[0] = float4(vRight, 0.f) * length(TransformMatrix[0]);
    TransformMatrix[1] = float4(vUp, 0.f) * length(TransformMatrix[1]);
    TransformMatrix[2] = float4(vLook, 0.f) * length(TransformMatrix[2]);

    vector vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

    matrix matVP;
    matVP = mul(g_ViewMatrix, g_ProjMatrix);
    Out.vPosition = mul(vPosition, matVP);
    Out.vTexUV = In.vTexUV;
    Out.vColor = In.vColor;

    Out.vSpriteUV = In.vSpriteUV;

    return Out;
}

VS_OUT_SPRITE_SOFT VS_SPRITE_SOFT(VS_IN_SPRITE In)
{
    VS_OUT_SPRITE_SOFT Out = (VS_OUT_SPRITE_SOFT)0;

    // billboard
    float3 vLook = normalize((g_vCamDirection * -1.f).xyz);
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
    float3 vUp = normalize(cross(vLook, vRight));

    In.vRight = float4(vRight, 0.f) * length(In.vRight);
    In.vUp = float4(vUp, 0.f) * length(In.vUp);
    In.vLook = float4(vLook, 0.f) * length(In.vLook);
    // **billboard

    matrix TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    vector vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

    matrix matWV, matWVP;
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexUV = In.vTexUV;
    Out.vColor = In.vColor;

    Out.vSpriteUV = In.vSpriteUV;

    Out.vProjPos = Out.vPosition;
    return Out;
}

struct PS_IN
{
    float4 vPosition     : SV_POSITION;
    float2 vTexUV        : TEXCOORD0;
    float4 vColor        : TEXCOORD1;
};

struct PS_IN_SOFT
{
    float4 vPosition     : SV_POSITION;
    float2 vTexUV        : TEXCOORD0;
    float4 vColor        : TEXCOORD1;
    float4 vProjPos      : TEXCOORD2;
};

struct PS_IN_SPRITE
{
    float4 vPosition     : SV_POSITION;
    float2 vTexUV        : TEXCOORD0;
    float4 vColor        : TEXCOORD1;

    float2 vSpriteUV     : TEXCOORD2;
};

struct PS_IN_SPRITE_SOFT
{
    float4 vPosition     : SV_POSITION;
    float2 vTexUV        : TEXCOORD0;
    float4 vColor        : TEXCOORD1;

    float2 vSpriteUV     : TEXCOORD2;

    float4 vProjPos      : TEXCOORD3;
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

    if (g_bXInverse)
        In.vTexUV.x = 1.f - In.vTexUV.x;

    if (g_bYInverse)
        In.vTexUV.y = 1.f - In.vTexUV.y;

    Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV + g_vDiffuseUV);
    Out.vColor *= In.vColor;

    vector vNoise = g_NoiseTexture.Sample(PointSampler, In.vTexUV + g_vNoiseUV);
    // vNoise.rgb = vNoise.rgb * 2 - 1;

    Out.vColor.rgb *= vNoise.rgb;
    Out.vColor.a *= g_MaskTexture.Sample(PointSampler, In.vTexUV + g_vMaskUV).g;

    if (Out.vColor.a < g_fDiscardRatio)
        discard;

    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;

    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}

PS_OUT PS_MAIN_ALPHADISCARD_SOFT(PS_IN_SOFT In)
{
    PS_OUT Out = (PS_OUT)0;

    if (g_bXInverse)
        In.vTexUV.x = 1.f - In.vTexUV.x;

    if (g_bYInverse)
        In.vTexUV.y = 1.f - In.vTexUV.y;

    Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV + g_vDiffuseUV);
    Out.vColor *= In.vColor;

    vector vNoise = g_NoiseTexture.Sample(PointSampler, In.vTexUV + g_vNoiseUV);
    // vNoise.rgb = vNoise.rgb * 2 - 1;

    float2 vTexUV;

    /* -1, 1 => 1, -1*/
    vTexUV.x = In.vProjPos.x / In.vProjPos.w;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w;

    /* 0, 0 => 1, 1*/
    vTexUV.x = vTexUV.x * 0.5f + 0.5f;
    vTexUV.y = vTexUV.y * -0.5f + 0.5f;

    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);

    float fViewZ = vDepthDesc.y * 300.f;

    Out.vColor.rgb *= vNoise.rgb;
    Out.vColor.a *= g_MaskTexture.Sample(PointSampler, In.vTexUV + g_vMaskUV).g;

    Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w);

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

    if (g_bXInverse)
        In.vTexUV.x = 1.f - In.vTexUV.x;

    if (g_bYInverse)
        In.vTexUV.y = 1.f - In.vTexUV.y;

    Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV + g_vDiffuseUV);
    Out.vColor *= In.vColor;

    vector vNoise = g_NoiseTexture.Sample(PointSampler, In.vTexUV + g_vNoiseUV);
    // vNoise.rgb = vNoise.rgb * 2 - 1;

    Out.vColor.rgb *= vNoise.rgb;
    Out.vColor.a *= g_MaskTexture.Sample(PointSampler, In.vTexUV + g_vMaskUV).r;

    if ((Out.vColor.r + Out.vColor.g + Out.vColor.b) <= g_fDiscardRatio)
        discard;

    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;

    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}

PS_OUT PS_MAIN_BLACKDISCARD_SOFT(PS_IN_SOFT In)
{
    PS_OUT Out = (PS_OUT)0;

    if (g_bXInverse)
        In.vTexUV.x = 1.f - In.vTexUV.x;

    if (g_bYInverse)
        In.vTexUV.y = 1.f - In.vTexUV.y;

    Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV + g_vDiffuseUV);
    Out.vColor *= In.vColor;

    vector vNoise = g_NoiseTexture.Sample(PointSampler, In.vTexUV + g_vNoiseUV);
    // vNoise.rgb = vNoise.rgb * 2 - 1;

    float2 vTexUV;

    /* -1, 1 => 1, -1*/
    vTexUV.x = In.vProjPos.x / In.vProjPos.w;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w;

    /* 0, 0 => 1, 1*/
    vTexUV.x = vTexUV.x * 0.5f + 0.5f;
    vTexUV.y = vTexUV.y * -0.5f + 0.5f;

    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);

    float fViewZ = vDepthDesc.y * 300.f;

    Out.vColor.rgb *= vNoise.rgb;
    Out.vColor.a *= g_MaskTexture.Sample(PointSampler, In.vTexUV + g_vMaskUV).g;

    Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w);

    if ((Out.vColor.r + Out.vColor.g + Out.vColor.b) <= g_fDiscardRatio)
        discard;

    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;

    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}

PS_OUT PS_SPRITE_BLACKDISCARD(PS_IN_SPRITE In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (g_bXInverse)
        In.vTexUV.x = 1.f - In.vTexUV.x;

    if (g_bYInverse)
        In.vTexUV.y = 1.f - In.vTexUV.y;
	
	// Sprite Image
	if (g_bUseMask)
	{
		Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV + g_vDiffuseUV);
		Out.vColor *= In.vColor;

		vector vColor = g_MaskTexture.Sample(PointClampSampler,
			float2(In.vTexUV.x / g_iNumFrameX + In.vSpriteUV.x,
				In.vTexUV.y / g_iNumFrameY + In.vSpriteUV.y));

		Out.vColor.a *= vColor.r;
	}
	else
	{
		Out.vColor = g_DiffuseTexture.Sample(PointClampSampler,
			float2(In.vTexUV.x / g_iNumFrameX + In.vSpriteUV.x,
				In.vTexUV.y / g_iNumFrameY + In.vSpriteUV.y));
		Out.vColor *= In.vColor;
	}

    if ((Out.vColor.r + Out.vColor.g + Out.vColor.b) <= g_fDiscardRatio)
        discard;
	
    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;
	
    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;
	
    return Out;
}

PS_OUT PS_SPRITE_ALPHADISCARD_SOFT(PS_IN_SPRITE_SOFT In)
{
    PS_OUT Out = (PS_OUT)0;

    if (g_bXInverse)
        In.vTexUV.x = 1.f - In.vTexUV.x;

    if (g_bYInverse)
        In.vTexUV.y = 1.f - In.vTexUV.y;

	// Sprite Image
	if (g_bUseMask)
	{
		Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV + g_vDiffuseUV);
		Out.vColor *= In.vColor;

		vector vColor = g_MaskTexture.Sample(PointClampSampler,
			float2(In.vTexUV.x / g_iNumFrameX + In.vSpriteUV.x,
				In.vTexUV.y / g_iNumFrameY + In.vSpriteUV.y));

		Out.vColor.a *= vColor.r;
	}
	else
	{
		Out.vColor = g_DiffuseTexture.Sample(PointClampSampler,
			float2(In.vTexUV.x / g_iNumFrameX + In.vSpriteUV.x,
				In.vTexUV.y / g_iNumFrameY + In.vSpriteUV.y));
		Out.vColor *= In.vColor;
	}

    float2 vTexUV;

    /* -1, 1 => 1, -1*/
    vTexUV.x = In.vProjPos.x / In.vProjPos.w;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w;

    /* 0, 0 => 1, 1*/
    vTexUV.x = vTexUV.x * 0.5f + 0.5f;
    vTexUV.y = vTexUV.y * -0.5f + 0.5f;

    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);

    float fViewZ = vDepthDesc.y * 300.f;

    Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w);

    if ((Out.vColor.r + Out.vColor.g + Out.vColor.b) <= g_fDiscardRatio)
        discard;

    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;

    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}

PS_OUT PS_SPRITE_ALPHADISCARD(PS_IN_SPRITE In)
{
    PS_OUT Out = (PS_OUT)0;

    if (g_bXInverse)
        In.vTexUV.x = 1.f - In.vTexUV.x;

    if (g_bYInverse)
        In.vTexUV.y = 1.f - In.vTexUV.y;

	// Sprite Image
	if (g_bUseMask)
	{
		Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV + g_vDiffuseUV);
		Out.vColor *= In.vColor;

		vector vColor = g_MaskTexture.Sample(PointClampSampler,
			float2(In.vTexUV.x / g_iNumFrameX + In.vSpriteUV.x,
				In.vTexUV.y / g_iNumFrameY + In.vSpriteUV.y));

		Out.vColor.a *= vColor.r;
	}
	else
	{
		Out.vColor = g_DiffuseTexture.Sample(PointClampSampler,
			float2(In.vTexUV.x / g_iNumFrameX + In.vSpriteUV.x,
				In.vTexUV.y / g_iNumFrameY + In.vSpriteUV.y));
		Out.vColor *= In.vColor;
	}

    if (Out.vColor.a <= g_fDiscardRatio)
        discard;

    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;

    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}

PS_OUT PS_SPRITE_BLACKDISCARD_SOFT(PS_IN_SPRITE_SOFT In)
{
    PS_OUT Out = (PS_OUT)0;

    if (g_bXInverse)
        In.vTexUV.x = 1.f - In.vTexUV.x;

    if (g_bYInverse)
        In.vTexUV.y = 1.f - In.vTexUV.y;

	// Sprite Image
	if (g_bUseMask)
	{
		Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV + g_vDiffuseUV);
		Out.vColor *= In.vColor;

		vector vColor = g_MaskTexture.Sample(PointClampSampler,
			float2(In.vTexUV.x / g_iNumFrameX + In.vSpriteUV.x,
				In.vTexUV.y / g_iNumFrameY + In.vSpriteUV.y));

		Out.vColor.a *= vColor.r;
	}
	else
	{
		Out.vColor = g_DiffuseTexture.Sample(PointClampSampler,
			float2(In.vTexUV.x / g_iNumFrameX + In.vSpriteUV.x,
				In.vTexUV.y / g_iNumFrameY + In.vSpriteUV.y));
        Out.vColor *= In.vColor;
	}

    float2 vTexUV;

    /* -1, 1 => 1, -1*/
    vTexUV.x = In.vProjPos.x / In.vProjPos.w;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w;

    /* 0, 0 => 1, 1*/
    vTexUV.x = vTexUV.x * 0.5f + 0.5f;
    vTexUV.y = vTexUV.y * -0.5f + 0.5f;

    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);

    float fViewZ = vDepthDesc.y * 300.f;

    Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w);

    if (Out.vColor.a <= g_fDiscardRatio)
        discard;

    if (g_bBloom)
        Out.vExtractBloom = Out.vColor;

    if (g_bGlow)
        Out.vExtractGlow = g_vGlowColor;

    return Out;
}

technique11 DefaultTechnique
{
    pass SpriteImage_BlackDiscard_DSSDefault // 0
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SPRITE();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_SPRITE_BLACKDISCARD();
    }

    pass Default_AlphaDiscard_DSSDefault // 1
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALPHADISCARD();
    }

    pass Default_BlackDiscard_DSSDefault // 2
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLACKDISCARD();
    }

    pass SpriteImage_AlphaDiscard_DSSDefault // 3
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SPRITE();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SPRITE_ALPHADISCARD();
    }

    pass Default_AlphaDiscardSoft_DSSDefault // 4
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SOFT();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALPHADISCARD_SOFT();
    }

    pass Default_BlackDiscardSoft_DSSDefault // 5
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SOFT();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLACKDISCARD_SOFT();
    }

    pass SpriteImage_AlphaDiscardSoft_DSSDefault // 6
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SPRITE_SOFT();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SPRITE_ALPHADISCARD_SOFT();
    }

    pass SpriteImage_BlackDiscardSoft_DSSDefault // 7
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SPRITE_SOFT();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SPRITE_BLACKDISCARD_SOFT();
    }

    pass SpriteImage_BlackDiscard_DSSSpecial // 8
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SPRITE();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SPRITE_BLACKDISCARD();
    }

    pass Default_AlphaDiscard_DSSSpecial // 9
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALPHADISCARD();
    }

    pass Default_BlackDiscard_DSSSpecial // 10
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLACKDISCARD();
    }

    pass SpriteImage_AlphaDiscard_DSSSpecial // 11
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SPRITE();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SPRITE_ALPHADISCARD();
    }

    pass Default_AlphaDiscardSoft_DSSSpecial // 12
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SOFT();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALPHADISCARD_SOFT();
    }

    pass Default_BlackDiscardSoft_DSSSpecial // 13
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SOFT();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLACKDISCARD_SOFT();
    }

    pass SpriteImage_AlphaDiscardSoft_DSSSpecial // 14
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SPRITE_SOFT();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SPRITE_ALPHADISCARD_SOFT();
    }

    pass SpriteImage_BlackDiscardSoft_DSSSpecial // 15
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_SPRITE_SOFT();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SPRITE_BLACKDISCARD_SOFT();
    }
}