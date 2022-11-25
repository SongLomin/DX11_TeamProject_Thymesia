
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix; //직교투영
matrix	g_ProjMatrixInv, g_ViewMatrixInv; //월드 <-> 스크린
matrix  g_PreCamViewMatrix, g_CamProjMatrix;//proj는 변하지 않는다고 가정

vector		g_vCamPosition;

vector		g_vLinearVelocity;
vector		g_vAngularVelocity;

texture2D	g_DepthTexture;
texture2D	g_OriginalRenderTexture;

float		g_BlurStrength =0.1f;

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
	vector		vColor : SV_TARGET0;	
};

PS_OUT PS_MAIN_MOTION_BLUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	float			fViewZ = vDepthDesc.y * 300.f;

	vector			vPixelWorldPos,vPixelPos;

	vPixelWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vPixelWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vPixelWorldPos.z = vDepthDesc.x;
	vPixelWorldPos.w = 1.0f;

	vPixelPos = vPixelWorldPos;
	    
	vPixelWorldPos *= fViewZ;

	vPixelWorldPos = mul(vPixelWorldPos, g_ProjMatrixInv);

	vPixelWorldPos = mul(vPixelWorldPos, g_ViewMatrixInv);
	
	matrix matVP = mul(g_PreCamViewMatrix, g_CamProjMatrix);

	vector vPrePixelPos = mul(vPixelWorldPos, matVP);
	vPrePixelPos /= vPrePixelPos.w;

	float2 vPixelVelocity = (vPrePixelPos - vPixelPos)*0.5f;
	float2 texCoord = In.vTexUV;

	vector vColor = vector(0.f,0.f,0.f,0.f);

	for (int i = 0; i < 10; ++i, texCoord += vPixelVelocity * g_BlurStrength*0.5f)
	{
		float4 currentColor = g_OriginalRenderTexture.Sample(ClampSampler, texCoord);
		vColor += currentColor;
	}

	Out.vColor = vColor / 10.f;

	//for (int i = 0; i < 13; ++i)
	//{
	//	float2 offset = vPixelDiff * (float(i) / 13.f - 0.5f)/** g_BlurStrength*/;
	//	vColor += g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV + offset)* BlurWeights[i];
	//}


	//Out.vColor = vColor;

	//float zOverW = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	//float4 H = float4(texCoord.x * 2 - 1, (1 - texCoord.y) * 2 - 1, zOverW, 1);
	//float4 D = mul(H, g_ViewProjectionInverseMatrix);
	//float4 worldPos = D / Dw;
	//float4 currentPos = H;
	//float4 previousPos = mul(worldPos, g_previousViewProjectionMatrix);
	//previousPos /= previousPos.w;
	//float2 velocity = (currentPos - previousPos) / 2. f;
	//float4 color = tex2D(sceneSampler, texCoord);
	//texCoord += velocity;
	//for (int i = 1; i < g_numSamples; ++i, texCoord += velocity) {
	//	float4 currentColor = tex2D(sceneSampler, texCoord);
	//	color += currentColor;
	//}
	//float4 finalColor = color / numSamples;
	
	return Out;
}

PS_OUT PS_MAIN_CHROMATIC(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 center = float2(0.5f, 0.5f);

	float2 BlurDir = In.vTexUV - center;

	vector vColor;

	vColor.r = g_OriginalRenderTexture.Sample(ClampSampler, In.vTexUV).r;
	vColor.g = g_OriginalRenderTexture.Sample(ClampSampler, In.vTexUV - BlurDir * g_BlurStrength * 0.5f).g;
	vColor.b = g_OriginalRenderTexture.Sample(ClampSampler, In.vTexUV - BlurDir* g_BlurStrength).b;

	//vColor *= (1.0 - g_BlurStrength * 0.5); //안해주면 그냥 밝아짐

	Out.vColor = vColor;

	return Out;
}


technique11 DefaultTechnique
{
	pass MotionBlur //0
	{
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MOTION_BLUR();
	}

	pass Chromatic_Aberration//1
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CHROMATIC();
	}
}