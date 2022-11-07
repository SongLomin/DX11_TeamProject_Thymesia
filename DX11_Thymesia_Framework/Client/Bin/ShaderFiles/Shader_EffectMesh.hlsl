#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;
texture2D g_DepthTexture;
texture2D g_MaskTexture;
texture2D g_GradientTexture;

float4 g_vColor;
float2 g_vUV;
float4 g_vGlowColor;

vector g_vCamDirection;
bool g_bBillboard;
bool g_bBloom;
bool g_bGlow;
bool g_bDiffuseMap;

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

// w�����⿬���� �����ϳ�. (In ���������̽�)
// ����Ʈ ��ȯ. (In ����Ʈ(��������ǥ))

// �����Ͷ�����(�ȼ������� �����Ѵ�. )

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

	/* -1, 1 => 1, -1 : ������������ ��ġ. */
    vTexUV.x = In.vProjPos.x / In.vProjPos.w;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w;

	/* 0, 0 => 1, 1 : �ؽ��� ������ ��ǥ. */
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

// w�����⿬���� �����ϳ�. (In ���������̽�)
// ����Ʈ ��ȯ. (In ����Ʈ(��������ǥ))

// �����Ͷ�����(�ȼ������� �����Ѵ�. )

PS_OUT PS_MAIN_NONE_DIFFUSE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    //float red = g_GradientTexture.Sample(DefaultSampler, In.vTexUV + g_vUV.yx).r;
    
    //Out.vColor.r = red;
    //Out.vColor.gb = pow(float2(1.f, 1.f) - Out.vColor.rr, float2(2.f, 2.f));
    //Out.vColor.r = 1.f;
    
    Out.vColor = g_vColor;
    
    if (g_bDiffuseMap)
    {
        Out.vColor *= g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV + g_vUV);
    }
    
    //Out.vColor.gb = Out.vColor.rr;

    vector MaskColor = g_MaskTexture.Sample(DefaultSampler, In.vTexUV + g_vUV);
    
    Out.vColor.a *= MaskColor.r;
	
    if (Out.vColor.a < 0.1f)
        discard;
    
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

PS_OUT PS_MAIN_NONE_DIFFUSE_CLAMP(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    //float red = g_GradientTexture.Sample(DefaultSampler, In.vTexUV + g_vUV.yx).r;
    
    //Out.vColor.r = red;
    //Out.vColor.gb = pow(float2(1.f, 1.f) - Out.vColor.rr, float2(2.f, 2.f));
    //Out.vColor.r = 1.f;
    
    Out.vColor = g_vColor;
    
    if (g_bDiffuseMap)
    {
        Out.vColor *= g_DiffuseTexture.Sample(ClampSampler, In.vTexUV + g_vUV);
    }
    
    //Out.vColor.gb = Out.vColor.rr;

    vector MaskColor = g_MaskTexture.Sample(ClampSampler, In.vTexUV + g_vUV);
    
    Out.vColor.a *= MaskColor.r;
	
    if (Out.vColor.a < 0.1f)
        discard;

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

technique11 DefaultTechnique
{
    pass Default
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass SoftEffect
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);
		
        VertexShader = compile vs_5_0 VS_MAIN_SOFT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SOFT();
    }
    
    pass NoneDiffuseEffect_UVDefault
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);
		
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONE_DIFFUSE();
    }

    pass NoneDiffuseEffect_UVClamp
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);
		
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONE_DIFFUSE_CLAMP();
    }


	/* pass Default
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}*/
}