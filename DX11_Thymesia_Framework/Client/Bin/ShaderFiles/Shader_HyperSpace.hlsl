
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4	g_vMaskPosition[48];
float	g_fMaskRadius[48];
//float4	g_vGlowColor;
float   g_fGlowColor;

float   g_fAhlpaScale;

texture2D	g_DiffuseTexture;
texture2D	g_GlowTexture;

bool		g_bBloom;
bool		g_bGlow;

struct		tagBoneMatrix
{
	matrix		BoneMatrices[256];
};

tagBoneMatrix		g_Bones;


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;	
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	//float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
    float4		vLocalPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float			fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix			BoneMatrix = g_Bones.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_Bones.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_Bones.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_Bones.BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    Out.vLocalPos = vPosition;
    

    Out.vPosition = mul(vPosition, matWVP);
	//Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;

	return Out;	
}
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	//float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	//float4		vWorldPos : TEXCOORD1;
    float4		vLocalPos : TEXCOORD1;

};

struct PS_OUT
{	
	vector		vColor : SV_TARGET0;	
    vector		vExtractBloom : SV_Target1;
    vector		vExtractGlow : SV_Target2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    vector vGlowDiffuse = g_GlowTexture.Sample(DefaultSampler, In.vTexUV);
    vGlowDiffuse.rgb += g_fGlowColor;

	Out.vColor = vMtrlDiffuse;
	
    for (int i = 0; i < 4; i++)
    {
        vector vDir = In.vLocalPos - g_vMaskPosition[i];
        float fDistance = length(vDir);
	
        if (fDistance < g_fMaskRadius[i])
        {
            Out.vColor.a = 1.f - (fDistance / g_fMaskRadius[i]);
			
            if (g_bBloom)
            {
                Out.vExtractBloom = Out.vColor;
            }
			
            if (g_bGlow)
            {
                Out.vExtractGlow = vGlowDiffuse;
                Out.vExtractGlow.a = Out.vColor.a * 0.5f;

            }
			
            return Out;
        }
    }
    Out.vColor = vector(0.f, 0.f, 0.f, 0.f);
	
	if(Out.vColor.a < 0.1f)
        discard;
	
    if (g_bBloom)
    {
        Out.vExtractBloom = Out.vColor;
    }
	
    if (g_bGlow)
    {
        Out.vExtractGlow = vGlowDiffuse;
        Out.vExtractGlow.a = Out.vColor.a * 0.5f;
    }

    return Out;
}

PS_OUT PS_MAIN_BREATH(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    vector vGlowDiffuse = g_GlowTexture.Sample(DefaultSampler, In.vTexUV);
    vGlowDiffuse.rgb += g_fGlowColor;

    Out.vColor = vMtrlDiffuse;
    Out.vColor.a *= g_fAhlpaScale;
	
    if (Out.vColor.a < 0.1f)
        discard;
	
    if (g_bBloom)
    {
        Out.vExtractBloom = Out.vColor;
    }
	
    if (g_bGlow)
    {
        Out.vExtractGlow = vGlowDiffuse;
        Out.vExtractGlow.a = Out.vColor.a * 0.5f;
    }

    return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
    pass NonCulling
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Start_Breath
    {
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BREATH();
    }
}