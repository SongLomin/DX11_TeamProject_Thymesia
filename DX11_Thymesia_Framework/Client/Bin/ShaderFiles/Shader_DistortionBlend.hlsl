
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D   g_OriginTexture;
texture2D   g_DistortionTexture;

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
	vector		vColor : SV_TARGET0;	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDistortion = g_DistortionTexture.Sample(DefaultSampler, In.vTexUV);

	if(0.f < vDistortion.r)
        Out.vColor = g_OriginTexture.Sample(DefaultSampler, In.vTexUV + (vDistortion.r * 0.1f));
	else
		Out.vColor = g_OriginTexture.Sample(DefaultSampler, In.vTexUV);
	
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
		PixelShader = compile ps_5_0 PS_MAIN();
	}

}