#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_vCamDirection;

texture2D g_DiffuseTexture;
texture2D g_NoiseTexture;
texture2D g_MaskTexture;
texture2D g_DissolveTexture;//dissovle

texture2D g_DepthTexture;
// TODO :  bDynamicNoiseOption temporary for test
bool   g_bDynamicNoiseOption;

// For. Dissolve
float  g_fDissolveAmount;

float3 g_vDissolveGradiationStartColor;
float3 g_vDissolveGradiationGoalColor;
float  g_fDissolveGradiationDistance;

// For. Directional Dissolve
float3 g_vDissolveStartPos;
float3 g_vDissolveDir;
float4 g_vShaderFlag = { 0.f, 0.f, 0.f, 0.f };

float4 g_vRimLightColor;

/**
* Wrap Weight for Textures
*  x : Diff, y : Noise, z : Mask, w : None
*/
vector g_vWrapWeight;


bool   g_bDiffuseWrap;
bool   g_bNoiseWrap;
bool   g_bMaskWrap;

float2 g_vUVDiff;
float2 g_vUVNoise;
float2 g_vUVMask;

float  g_fDiscardRatio;

float4 g_vColor;

bool   g_bBillboard;

bool   g_bBloom;

bool   g_bGlow;
float4 g_vGlowColor;

// Vertex Shaders //

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV    : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV    : TEXCOORD0;
};

struct VS_OUT_SOFT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV    : TEXCOORD0;
	float4 vProjPos  : TEXCOORD1;
};

struct VS_OUT_DIRECTIONAL_DISSOLVE
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV    : TEXCOORD0;
	float3 vLocalPos : TEXCOORD1;
};

struct VS_OUT_SOFT_DIRECTIONAL_DISSOLVE
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV    : TEXCOORD0;
	float4 vProjPos  : TEXCOORD1;
	float3 vLocalPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

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
	VS_OUT_SOFT Out = (VS_OUT_SOFT)0;

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
	Out.vProjPos = Out.vPosition;

	return Out;
}
//  Vertex Shaders  //
VS_OUT_DIRECTIONAL_DISSOLVE VS_MAIN_DIRECTIONAL_DISSOLVE(VS_IN In)
{
	VS_OUT_DIRECTIONAL_DISSOLVE Out = (VS_OUT_DIRECTIONAL_DISSOLVE)0;

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
	Out.vLocalPos = In.vPosition;

	return Out;
}

VS_OUT_SOFT_DIRECTIONAL_DISSOLVE VS_MAIN_SOFT_DIRECTIONAL_DISSOLVE(VS_IN In)
{
	VS_OUT_SOFT_DIRECTIONAL_DISSOLVE Out = (VS_OUT_SOFT_DIRECTIONAL_DISSOLVE)0;

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
	Out.vProjPos = Out.vPosition;
	Out.vLocalPos = In.vPosition;

	return Out;
}

//  Pixel  Shaders  //
struct PS_IN
{
	float4 vPosition     : SV_POSITION;
	float2 vTexUV        : TEXCOORD0;
};

struct PS_IN_DIRECTIONAL_DISSOLVE
{
	float4 vPosition     : SV_POSITION;
	float2 vTexUV        : TEXCOORD0;
	float3 vLocalPos     : TEXCOORD1;
};

struct PS_IN_SOFT_DIRECTIONAL_DISSOLVE
{
	float4 vPosition     : SV_POSITION;
	float2 vTexUV        : TEXCOORD0;
	float4 vProjPos      : TEXCOORD1;
	float3 vLocalPos     : TEXCOORD2;
};

struct PS_IN_SOFT
{
	float4 vPosition     : SV_POSITION;
	float2 vTexUV        : TEXCOORD0;
	float4 vProjPos      : TEXCOORD1;
};

struct PS_OUT
{
	vector vColor        : SV_TARGET0;
	vector vExtractBloom : SV_Target1;
	vector vExtractGlow  : SV_Target2;
	vector vShaderFlag   : SV_TARGET3;
};

struct PS_OUT_DISTORTION
{
	vector vColor        : SV_TARGET0;
};

PS_OUT PS_MAIN_SOFT(PS_IN_SOFT In)
{
	PS_OUT Out = (PS_OUT)0;

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

	float2 vTexUV;

	vTexUV.x = In.vProjPos.x / In.vProjPos.w;
	vTexUV.y = In.vProjPos.y / In.vProjPos.w;

	vTexUV.x = vTexUV.x * 0.5f + 0.5f;
	vTexUV.y = vTexUV.y * -0.5f + 0.5f;

	vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);
	float fViewZ = vDepthDesc.y * 300.f;
	Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w);

	if (g_bBloom)
		Out.vExtractBloom = Out.vColor;

	if (g_bGlow)
		Out.vExtractGlow = g_vGlowColor;

	Out.vShaderFlag = g_vShaderFlag;

	return Out;
}

PS_OUT PS_DEFAULT(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

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

	Out.vShaderFlag = g_vShaderFlag;

	return Out;
}

PS_OUT_DISTORTION PS_DISTORTION(PS_IN In)
{
	PS_OUT_DISTORTION Out = (PS_OUT_DISTORTION)0;

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


PS_OUT PS_EXTRACTBRIGHT(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

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
	if (0.8f < Out.vColor.a)
	{
		Out.vExtractBloom = Out.vColor;
		Out.vExtractGlow = g_vGlowColor;
	}


	Out.vShaderFlag = g_vShaderFlag;

	return Out;
}

PS_OUT PS_MAIN_NORMAL_DISSOLVE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	// normal dissolve
	float DissolveDesc = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV).r;

	clip(DissolveDesc - g_fDissolveAmount);

	if (g_fDissolveAmount + g_fDissolveGradiationDistance >= DissolveDesc)
	{
		float fLerpRatio = (DissolveDesc - g_fDissolveAmount) / g_fDissolveGradiationDistance;
		Out.vColor = vector(lerp(g_vDissolveGradiationStartColor, g_vDissolveGradiationGoalColor, fLerpRatio), 1.f);
		Out.vShaderFlag = vector(0.f, 0.f, 1.f, 0.f);
	}
	else
	{
		Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
		Out.vColor *= g_vColor;
	}


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

float IsIn_Range(float fMin, float fMax, float fValue)
{
    return (fMin <= fValue) && (fMax >= fValue);
}


PS_OUT PS_MAIN_NORMAL_DIRECTIONAL_DISSOLVE(PS_IN_DIRECTIONAL_DISSOLVE In)
{
	PS_OUT Out = (PS_OUT)0;

	//directional dissolve
	float3 vPixelDir = In.vLocalPos.xyz - g_vDissolveStartPos;
	vPixelDir = normalize(vPixelDir);
	float3 vDissolveDir = normalize(g_vDissolveDir);

	float fDotValue = dot(vPixelDir.xyz, vDissolveDir);
	fDotValue = fDotValue * 0.5f + 0.5f;

	clip(fDotValue - g_fDissolveAmount);

	float fDiff = fDotValue - g_fDissolveAmount;
	float fStepValue = IsIn_Range(0.f, g_fDissolveGradiationDistance, fDiff);

	Out.vColor = fStepValue * vector(g_vDissolveGradiationStartColor, 1.f) +
		IsIn_Range(g_fDissolveGradiationDistance, 1.f, fDiff) * g_vColor;

	Out.vShaderFlag = fStepValue* vector(0.f, 0.f, 1.f, 0.f);

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

PS_OUT PS_MAIN_NORMAL_DISSOLVE_SOFT(PS_IN_SOFT In)
{
	PS_OUT Out = (PS_OUT)0;

	// normal dissolve
	float DissolveDesc = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV).r;

	clip(DissolveDesc - g_fDissolveAmount);

	vector vTexDiff;

	if (g_fDissolveAmount + g_fDissolveGradiationDistance >= DissolveDesc)
	{
		float fLerpRatio = clamp((DissolveDesc - g_fDissolveAmount) / g_fDissolveGradiationDistance, 0.f, 1.f);
		Out.vColor = vector(lerp(g_vDissolveGradiationStartColor, g_vDissolveGradiationGoalColor, fLerpRatio), 1.f);
		Out.vShaderFlag = vector(0.f, 0.f, 1.f, 0.f);
	}
	else
	{
		Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
		Out.vColor *= g_vColor;
	}

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

	Out.vColor.rgb *= vNoise.rgb;
	Out.vColor.a *= vMask.r;

	if (g_fDiscardRatio > Out.vColor.a)
		discard;

	float2 vTexUV;

	vTexUV.x = In.vProjPos.x / In.vProjPos.w;
	vTexUV.y = In.vProjPos.y / In.vProjPos.w;

	vTexUV.x = vTexUV.x * 0.5f + 0.5f;
	vTexUV.y = vTexUV.y * -0.5f + 0.5f;

	vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);
	float fViewZ = vDepthDesc.y * 300.f;
	Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w);

	if (g_bBloom)
		Out.vExtractBloom = Out.vColor;

	if (g_bGlow)
		Out.vExtractGlow = g_vGlowColor;

	return Out;
}

PS_OUT PS_MAIN_NORMAL_DIRECTIONAL_DISSOLVE_SOFT(PS_IN_SOFT_DIRECTIONAL_DISSOLVE In)
{
	PS_OUT Out = (PS_OUT)0;

	//directional dissolve
	float3 vPixelDir = In.vLocalPos.xyz - g_vDissolveStartPos;
	vPixelDir = normalize(vPixelDir);
	float3 vDissolveDir = normalize(g_vDissolveDir);

	float fDotValue = dot(vPixelDir.xyz, vDissolveDir);
	fDotValue = fDotValue * 0.5f + 0.5f;

	clip(fDotValue - g_fDissolveAmount);

	float fDiff = fDotValue - g_fDissolveAmount;
	float fStepValue = IsIn_Range(0.f, g_fDissolveGradiationDistance, fDiff);

	Out.vColor = fStepValue * vector(g_vDissolveGradiationStartColor, 1.f) +
		IsIn_Range(g_fDissolveGradiationDistance, 1.f, fDiff) * g_vColor;

	Out.vShaderFlag = fStepValue* vector(0.f, 0.f, 1.f, 0.f);

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

	Out.vColor.rgb *= vNoise.rgb;
	Out.vColor.a *= vMask.r;

	if (g_fDiscardRatio > Out.vColor.a)
		discard;

	float2 vTexUV;

	vTexUV.x = In.vProjPos.x / In.vProjPos.w;
	vTexUV.y = In.vProjPos.y / In.vProjPos.w;

	vTexUV.x = vTexUV.x * 0.5f + 0.5f;
	vTexUV.y = vTexUV.y * -0.5f + 0.5f;

	vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);
	float fViewZ = vDepthDesc.y * 300.f;
	Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w);

	if (g_bBloom)
		Out.vExtractBloom = Out.vColor;

	if (g_bGlow)
		Out.vExtractGlow = g_vGlowColor;

	return Out;
}


// Shader Passes //
technique11 DefaultTechnique
{
	pass Default // 0
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
		SetRasterizerState(RS_NonCulling);

		VertexShader = compile vs_5_0 VS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DEFAULT();
	}

	pass Distortion // 1
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
		SetRasterizerState(RS_NonCulling);

		VertexShader = compile vs_5_0 VS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISTORTION();
	}

	pass SoftEffect // 2
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_NonCulling);

		VertexShader = compile vs_5_0 VS_MAIN_SOFT();
		HullShader = NULL;
		DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SOFT();
	}

	pass ExtractBright//3
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_NonCulling);

		VertexShader = compile vs_5_0 VS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_EXTRACTBRIGHT();
	}

	pass Dissolve//4
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
		SetRasterizerState(RS_NonCulling);

		VertexShader = compile vs_5_0 VS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL_DISSOLVE();
	}


	pass Directional_Dissolve//5
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
		SetRasterizerState(RS_NonCulling);

		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL_DISSOLVE();
		HullShader = NULL;
		DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL_DIRECTIONAL_DISSOLVE();
	}

	pass Dissolve_Soft //6
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
		SetRasterizerState(RS_NonCulling);

		VertexShader = compile vs_5_0 VS_MAIN_SOFT();
		HullShader = NULL;
		DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL_DISSOLVE_SOFT();
	}


	pass Directional_Dissolve_Soft //7
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
		SetRasterizerState(RS_NonCulling);

		VertexShader = compile vs_5_0 VS_MAIN_SOFT_DIRECTIONAL_DISSOLVE();
		HullShader = NULL;
		DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL_DIRECTIONAL_DISSOLVE_SOFT();
	}
}
// Shader Passes //