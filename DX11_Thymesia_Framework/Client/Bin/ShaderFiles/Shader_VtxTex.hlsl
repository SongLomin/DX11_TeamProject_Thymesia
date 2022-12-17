
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_MaskTexture;
texture2D	g_DepthTexture;

float g_fAhlpaScale;
float g_MaskAhlpaScale;
float2 g_MaskUV;
float4	g_vColor;

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

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

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
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    //Out.vColor.a = 1.f;
    Out.vColor.a = Out.vColor.a * g_fAhlpaScale;
	
	/*if (Out.vColor.a < 0.1f)
	discard;*/

	return Out;
}

struct VS_OUT_SOFT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

VS_OUT_SOFT VS_MAIN_SOFT(VS_IN In)
{
	VS_OUT_SOFT		Out = (VS_OUT_SOFT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN_SOFT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

PS_OUT PS_MAIN_SOFT(PS_IN_SOFT In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float2		vTexUV;

	/* -1, 1 => 1, -1 : 투영공간상의 위치. */
	vTexUV.x = In.vProjPos.x / In.vProjPos.w;
	vTexUV.y = In.vProjPos.y / In.vProjPos.w;

	/* 0, 0 => 1, 1 : 텍스쳐 유브이 좌표. */
	vTexUV.x = vTexUV.x * 0.5f + 0.5f;
	vTexUV.y = vTexUV.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);

	float		fViewZ = vDepthDesc.y * 300.f;

	Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w);

	return Out;

}

PS_OUT PS_MAIN_ADD_MASK(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    float4 MaskColor = g_MaskTexture.Sample(DefaultSampler, In.vTexUV + g_MaskUV);
	
    Out.vColor += MaskColor.a;
	
    Out.vColor.a = Out.vColor.a * g_MaskAhlpaScale;
	
	/*if (Out.vColor.a < 0.1f)
	discard;*/

    return Out;
}


PS_OUT PS_MAIN_CUSTOMUI(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//Out.vColor.a = 1.f;
	//Out.vColor.a = Out.vColor.a * g_fAhlpaScale;

	/*if (Out.vColor.a < 0.1f)
	discard;*/

	return Out;
}

PS_OUT PS_MAIN_CUSTOMUI_FADE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.a *= g_vColor.a;

	//if (Out.vColor.a < 0.1)
	//	discard;

	//Out.vColor.a = 1.f;
	//Out.vColor.a = Out.vColor.a * g_fAhlpaScale;

	/*if (Out.vColor.a < 0.1f)
	discard;*/

	return Out;
}

PS_OUT PS_MAIN_CUSTOMUI_LOGO_BG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.rgb *= 7.f;

	//if (Out.vColor.a < 0.1)
	//	discard;

	//Out.vColor.a = 1.f;
	//Out.vColor.a = Out.vColor.a * g_fAhlpaScale;

	/*if (Out.vColor.a < 0.1f)
	discard;*/

	return Out;
}



technique11 DefaultTechnique
{
	//0
	pass Default
	{
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	//1
	pass SoftEffect
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);
		
        VertexShader = compile vs_5_0 VS_MAIN_SOFT();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SOFT();
	}
	
	//2
    pass Backgorund
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

	//3
    pass AddMask
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ADD_MASK();
    }

	//4
	pass CustomUI
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CUSTOMUI();
	}
	//5
	pass CustomUI_Font_BG_Alpha
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CUSTOMUI_FADE();
	}
	
	//6
	pass CustomUI_Logo_BG
	{
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CUSTOMUI_LOGO_BG();
	}
	/* pass Default
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}*/
}
//PBR Shader Code
//if (_WorldSpaceLightPos0.w == 1)
//return floa  t4(0.0, 0.0, 0.0, 0.0);
//
//// Just for mapping the 2d texture onto a sphere
//float2 uv = i.uv;
//
//// VECTORS
//
//// Assuming this pass goes only for directional lights
//float3 lightVec = normalize(_WorldSpaceLightPos0.xyz);
//
//float3 viewVec = normalize(_WorldSpaceCameraPos.xyz - i.worldPos);
//float3 halfVec = normalize(lightVec + viewVec);
//
//// Calculate the tangent matrix if normal mapping is applied
//float3x3 tangentMatrix = transpose(float3x3(i.tangent, i.bitangent, i.normal));
//float3 normal = mul(tangentMatrix, tex2D(_NormalMap, uv).xyz * 2 - 1);
//
//float3 reflectVec = -reflect(viewVec, normal);
//
//// DOT PRODUCTS
//float NdotL = max(dot(i.normal, lightVec), 0.0);
//float NdotH = max(dot(i.normal, halfVec), 0.0);
//float HdotV = max(dot(halfVec, viewVec), 0.0);
//float NdotV = max(dot(i.normal, viewVec), 0.0);
//float HdotT = dot(halfVec, i.tangentLocal);//trowbridgeReitzAnisotropicNDF 구하는 용으로 사용 이 ndf 쓰지 않으면 그냥 지워도됨
//float HdotB = dot(halfVec, i.bitangentLocal);//trowbridgeReitzAnisotropicNDF
//
//// TEXTURE SAMPLES
//float3 albedo = sRGB2Lin(tex2D(_MainTex, uv));
//
//// PBR PARAMETERS
//
//// This assumes that the maximum param is right if both are supplied (range and map)
//float roughness = saturate(max(_Roughness + EPS, tex2D(_RoughnessMap, uv)).r);
//float metalness = saturate(max(_Metalness + EPS, tex2D(_MetalnessMap, uv)).r);
//float occlusion = saturate(tex2D(_OcclusionMap, uv).r);
//
//float3 F0 = lerp(float3(0.04, 0.04, 0.04), _FresnelColor * albedo, metalness);
//
//float D = trowbridgeReitzNDF(NdotH, roughness);
//D = trowbridgeReitzAnisotropicNDF(NdotH, roughness, _Anisotropy, HdotT, HdotB);
//float3 F = fresnel(F0, NdotV, roughness);
//float G = schlickBeckmannGAF(NdotV, roughness) * schlickBeckmannGAF(NdotL, roughness);
//
//// DIRECT LIGHTING
//
//// Normals from normal map
//float lambertDirect = max(dot(normal, lightVec), 0.0);
//
//float3 directRadiance = _LightColor0.rgb * occlusion;
//
//// INDIRECT LIGHTING
//float3 diffuseIrradiance = sRGB2Lin(UNITY_SAMPLE_TEXCUBE_LOD(unity_SpecCube0, normal, UNITY_SPECCUBE_LOD_STEPS).rgb) * occlusion;
//float3 specularIrradiance = sRGB2Lin(UNITY_SAMPLE_TEXCUBE_LOD(unity_SpecCube0, reflectVec, roughness * UNITY_SPECCUBE_LOD_STEPS).rgb) * occlusion;
//
//// DIFFUSE COMPONENT
//float3 diffuseDirectTerm = lambertDiffuse(albedo) * (1 - F) * (1 - metalness) * _AlbedoColor;
//
//// SPECULAR COMPONENT
//float3 specularDirectTerm = G * D * F / (4 * NdotV * NdotL + EPS);
//
//// DIRECT BRDF OUTPUT
//float3 brdfDirectOutput = (diffuseDirectTerm + specularDirectTerm) * lambertDirect * directRadiance;
//
//// Add constant ambient (to boost the lighting, only temporary)
//float3 ambientDiffuse = diffuseIrradiance * lambertDiffuse(albedo) * (1 - F) * (1 - metalness);
//
//// For now the ambient specular looks quite okay, but it isn't physically correct
//// TODO: try importance sampling the NDF from the environment map (just for testing & performance measuring)
//// TODO: implement the split-sum approximation (UE4 paper)
//float3 ambientSpecular = specularIrradiance * F;
//
//return float4(gammaCorrection(brdfDirectOutput + ambientDiffuse + ambientSpecular), 1.0);