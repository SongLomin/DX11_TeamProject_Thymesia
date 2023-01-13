
matrix g_WorldMatrix    , g_ViewMatrix, g_ProjMatrix;
matrix g_CamProjMatrix, g_CamViewMatrix;

matrix g_ProjMatrixInv  , g_ViewMatrixInv;
matrix g_LightViewMatrix, g_LightProjMatrix;
matrix g_DynamicLightViewMatrix, g_DynamicLightProjMatrix;

// For Light Render
vector g_vLightDir;
vector g_vLightPos;
float  g_fRange;

float g_fCutOff;
float g_fOuterCutOff;
       
vector g_vCamPosition;
       
vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;
float g_fLightIntensity;
vector g_vLightFlag;
       
vector g_vMtrlAmbient  = vector(1.f, 1.f, 1.f, 1.f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

float3 g_IrradianceColorScale;

vector g_vFogColor;
float g_fFogRange;

texture2D g_ORMTexture;
texture2D g_SpecularTexture;
texture2D g_DepthTexture;
texture2D g_DiffuseTexture;
texture2D g_AmbientTexture;
texture2D g_ShadeTexture;
texture2D g_NormalTexture;
texture2D g_LightFlagTexture;
texture2D g_ShadowDepthTexture;
texture2D g_StaticShadowDepthTexture;
texture2D g_Texture;
texture2D g_ViewShadow;
texture2D g_FogTexture;
texture2D g_XBlurTexture;
texture2D g_ExtractBloomTexture;
texture2D g_OriginalRenderTexture;
texture2D g_ShaderFlagTexture;
texture2D g_HBAOTexture;

texture2D g_RimLightTexture;

textureCUBE g_IrradianceTexture;
textureCUBE g_PreFilterTexture;
texture2D g_BRDFTexture;

texture2D g_PostEffectMaskTexture;
texture2D g_BloomTexture;

texture2D g_GlowTexture;
texture2D g_OriginalEffectTexture;

// For Motion Blur
float g_fBlurWidth;

float g_fExposure;

float g_fFar = 300;

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
};

sampler ClampSampler = sampler_state
{
    filter   = min_mag_linear_mip_point;
    AddressU = clamp;
    AddressV = clamp;
};

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

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
    
	matrix matWV, matWVP;

	matWV  = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV    : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor    : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);	
	return Out;
}

struct PS_OUT_LIGHT
{
	vector vSpecular : SV_TARGET0;
    vector vAmbient  : SV_TARGET1;
};

float trowbridgeReitzNDF(float NdotH, float roughness)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float NdotH2 = NdotH * NdotH;
    float denom = ((alpha2 - 1) * NdotH2 + 1);
    float denominator = 3.141592265359 * denom* denom;
    return alpha2 / denominator;
}

float3 fresnel(float3 F0, float NdotV, float roughness)
{
    return F0 + (max(1.0 - roughness, F0) - F0) * pow(1 - NdotV, 5);
}

float3 fresnel(float3 F0, float NdotV)
{
    
    return F0 + (1 - F0) * pow(1 - NdotV, 5);
}

float schlickBeckmannGAF(float dotProduct, float roughness)
{
   // float alpha = roughness * roughness;
   // float k = alpha * 0.797884560803;  // sqrt(2 / PI)
    float fRoughness = (roughness + 1.f);
    float k = (fRoughness * fRoughness) / 8.f;
    
    return dotProduct / (dotProduct * (1 - k) + k);
}

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(1.0 - roughness, F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

	/* 방향성광원의 정보와 노멀 타겟에 담겨있는 노멀과의 빛연산을 수행한다. */
    vector vDiffuseColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    vector vNormalDesc    = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector vDepthDesc     = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
    //vector vLightFlagDesc = g_LightFlagTexture.Sample(DefaultSampler, In.vTexUV);
    vector vORMDesc     = g_ORMTexture.Sample(DefaultSampler, In.vTexUV);
   
    vDiffuseColor = pow(vDiffuseColor, 2.2f);
    vDiffuseColor.a = 1.f;

	//결과가 0이라면 일치하는 라이트 플래그가 없다. 그리지 않는다.
   // vLightFlagDesc *= g_vLightFlag;
	
    float fMaxScalar = 0.f;
	
    //for (uint i = 0; i < 4; ++i)
    //    fMaxScalar = max(fMaxScalar, vLightFlagDesc[i]);
	
    //if (fMaxScalar < 0.05f)
    //    discard;

	vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    vNormal = normalize(vNormal);
    float fViewZ = vDepthDesc.y * g_fFar;
    /* 0 -> -1, 1 -> 1*/
    vector vWorldPos;

    /* 투영스페이스 상의 위치르 ㄹ구한다. */
    /* 뷰스페이스 상 * 투영행렬 / w 까지 위치를 구한다. */
    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.0f;

    /* 뷰스페이스 상 * 투영행렬까지 곱해놓은 위치를 구한다. */
    vWorldPos *= fViewZ;

    /* 뷰스페이스 상  위치를 구한다. */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

    /* 월드페이스 상  위치를 구한다. */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    vector vLook = normalize(g_vCamPosition- vWorldPos);
    
 
    vLook.a = 0.f;
    float fOcclusion = vORMDesc.x;
    float fRoughness = vORMDesc.y;
    float fMetalness = vORMDesc.z;
    
    if (fRoughness > 0.f || fMetalness > 0.f || fOcclusion > 0.f)
    {
        fRoughness = max(fRoughness, 0.001f);
        fMetalness = max(fMetalness, 0.001f);
        fOcclusion = max(fOcclusion, 0.001f);
        
        vector vHalfVec = normalize(vLook + normalize(g_vLightDir) * -1.f);

        float NdotL = max(dot(vNormal, normalize(g_vLightDir) * -1.f), 0.0);
        float NdotH = max(dot(vNormal, vHalfVec), 0.0);
        float NdotV = max(dot(vNormal, vLook), 0.0);
        float HdotV = max(dot(vHalfVec, vLook), 0.0);

        float3 vMetalic = lerp(float3(0.04f, 0.04f, 0.04f), vDiffuseColor.xyz, fMetalness);

        float NDF = trowbridgeReitzNDF(NdotH, fRoughness);
        float3 F = fresnel(vMetalic, HdotV, fRoughness);
        float G = schlickBeckmannGAF(NdotV, fRoughness) * schlickBeckmannGAF(NdotL, fRoughness);

        vector kS = vector(F, 0.f);
        vector kD = vector(1.f, 1.f, 1.f, 0.f) - kS;
        kD *= 1.f - fMetalness;

        vector vNumerator = kS * NDF * G;
        float fDenominator = 4.f * NdotV * NdotL;
        vector vSpecular = vNumerator / max(fDenominator, 0.001f);

        vector vSpecularAcc = vSpecular * NdotL * g_vLightDiffuse * fOcclusion;
        vector vAmbientColor = vDiffuseColor /*/ 3.141592265359*/ * kD  * NdotL * g_vLightDiffuse;
               

        Out.vSpecular = vSpecularAcc * g_fLightIntensity;
        Out.vSpecular.a = 0.f;
        
        Out.vAmbient = vAmbientColor * g_fLightIntensity;
        Out.vAmbient.a = 1.f;
    }
    else
    {    //Out.vShade = vShadeDesc;

        vector vResult = g_vLightDiffuse * saturate(saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient));

        if (vResult.r < 0.05f && vResult.g < 0.05f && vResult.b < 0.05f)
            discard;

        Out.vAmbient = vResult * g_fLightIntensity;
        Out.vAmbient.a = 1.f;

        vector vReflect = reflect(normalize(g_vLightDir), vNormal);


        Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 20.f) * g_fLightIntensity;
        Out.vSpecular.a = 0.f;
    }
    
    //Out.vAmbient = pow(Out.vAmbient, 2.2f);
    //Out.vSpecular = pow(Out.vSpecular, 2.2f);
       
	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHT Out = (PS_OUT_LIGHT)0;

	/* 방향성광원의 정보와 노멀 타겟에 담겨있는 노멀과의 빛연산을 수행한다. */
    vector vDiffuseColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector vNormalDesc    = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector vDepthDesc     = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
    vector vORMDesc = g_ORMTexture.Sample(DefaultSampler, In.vTexUV);

   // vLightFlagDesc *= g_vLightFlag;
	
    float fMaxScalar = 0.f;

    float fViewZ = vDepthDesc.y * g_fFar;
	/* 0 -> -1, 1 -> 1*/
	vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector vWorldPos;

	/* 투영스페이스 상의 위치르 ㄹ구한다. */
	/* 뷰스페이스 상 * 투영행렬 / w 까지 위치를 구한다. */
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.0f;

	/* 뷰스페이스 상 * 투영행렬까지 곱해놓은 위치를 구한다. */
	vWorldPos *= fViewZ;

	/* 뷰스페이스 상  위치를 구한다. */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드페이스 상  위치를 구한다. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector	vLightDir = vWorldPos - g_vLightPos;
    float fDistance =  length(vLightDir); 
    
    float fDenom = fDistance / g_fRange;
    float fAtt = 1.f / (1.f + fDenom * fDenom);
    
    clip(fAtt - 0.01f);
    
    fAtt *= g_fLightIntensity;
    vector vLook = normalize(g_vCamPosition - vWorldPos);
   
    float fRoughness = vORMDesc.y;
    float fMetalness = vORMDesc.z;
    float fOcclusion = vORMDesc.x;

    if (fRoughness > 0.f || fMetalness > 0.f || fOcclusion > 0.f)
    {
        fRoughness = max(fRoughness, 0.001f);
        fMetalness = max(fMetalness, 0.001f);
        fOcclusion = max(fOcclusion, 0.001f);
    
        vector vHalfVec = normalize(vLook + normalize(vLightDir) * -1.f);

        float NdotL = max(dot(vNormal, normalize(vLightDir) * -1.f), 0.0);
        float NdotH = max(dot(vNormal, vHalfVec), 0.0);
        float NdotV = max(dot(vNormal, vLook), 0.0);
        float HdotV = max(dot(vHalfVec, vLook), 0.0);
    
        float3 vMetalic = lerp(float3(0.04f, 0.04f, 0.04f), vDiffuseColor.xyz, fMetalness);

        float NDF = trowbridgeReitzNDF(NdotH, fRoughness);
        float3 F = fresnel(vMetalic, HdotV, fRoughness);
        float G = schlickBeckmannGAF(NdotV, fRoughness) * schlickBeckmannGAF(NdotL, fRoughness);

        vector kS = vector(F, 0.f);
        vector kD = vector(1.f, 1.f, 1.f, 0.f) - kS;
        kD *= 1.f - fMetalness;

        vector vNumerator = kS * NDF * G;
        float fDenominator = 4.f * NdotV * NdotL;
        vector vSpecular = vNumerator / max(fDenominator, 0.001f);

        vector vSpecularAcc = vSpecular * NdotL * g_vLightDiffuse * fAtt /** fOcclusion*/;
        vector vAmbientColor = kD * vDiffuseColor / 3.141592265359 * fOcclusion * NdotL * g_vLightDiffuse * fAtt;

        Out.vSpecular = vSpecularAcc;
        Out.vSpecular.a = 0.f;

        Out.vAmbient = vAmbientColor;
        Out.vAmbient.a = 1.f;
      
    }
    else
    {
    //Out.vShade = vShadeDesc;
        vector vResult = g_vLightDiffuse * saturate(saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;

        if (vResult.r < 0.05f && vResult.g < 0.05f && vResult.b < 0.05f)
            discard;

        Out.vAmbient = vResult * fAtt;
        Out.vAmbient.a = 1.f;

        vector vReflect = reflect(normalize(vLightDir), vNormal);
   
        Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 20.f) * fAtt;
        Out.vSpecular.a = 0.f;
    }
   
    //Out.vAmbient = pow(Out.vAmbient, 2.2f);
    //Out.vSpecular = pow(Out.vSpecular, 2.2f);
   
    return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_SPOTLIGHT(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT)1;

	/* 방향성광원의 정보와 노멀 타겟에 담겨있는 노멀과의 빛연산을 수행한다. */
    vector vDiffuseColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
    vector vORMDesc = g_ORMTexture.Sample(DefaultSampler, In.vTexUV);

    float fViewZ = vDepthDesc.y * g_fFar;
	/* 0 -> -1, 1 -> 1*/
    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector vWorldPos;

	/* 투영스페이스 상의 위치르 ㄹ구한다. */
	/* 뷰스페이스 상 * 투영행렬 / w 까지 위치를 구한다. */
    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.0f;

	/* 뷰스페이스 상 * 투영행렬까지 곱해놓은 위치를 구한다. */
    vWorldPos *= fViewZ;

	/* 뷰스페이스 상  위치를 구한다. */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드페이스 상  위치를 구한다. */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vLightDir = vWorldPos - g_vLightPos;

    float fDistance = length(vLightDir);

    float fDenom = fDistance / g_fRange;
    float fAtt = 1.f / (1.f + fDenom * fDenom);
 
    clip(fAtt - 0.0001f);
    
    fAtt *= g_fLightIntensity;
    
    float fTheta = dot(normalize(vLightDir), normalize(g_vLightDir)); 
    
    float fEpsilon = max(0.f, g_fCutOff - g_fOuterCutOff);
    float fCutoffIntensity = (fTheta - g_fOuterCutOff) / fEpsilon;
    clip(fCutoffIntensity);
   
    fAtt *= fCutoffIntensity;
   
    vector vLook = normalize(g_vCamPosition - vWorldPos);
    
    //Out.vShade = vShadeDesc;
    float fRoughness = vORMDesc.y;
    float fMetalness = vORMDesc.z;
    float fOcclusion = vORMDesc.x;

    if (fRoughness > 0.f || fMetalness > 0.f || fOcclusion > 0.f)
    {
        fRoughness = max(fRoughness, 0.001f);
        fMetalness = max(fMetalness, 0.001f);
        fOcclusion = max(fOcclusion, 0.001f);
    
        vector vHalfVec = normalize(vLook + normalize(g_vLightDir) * -1.f);

        float NdotL = max(dot(vNormal, normalize(g_vLightDir) * -1.f), 0.0);
        float NdotH = max(dot(vNormal, vHalfVec), 0.0);
        float NdotV = max(dot(vNormal, vLook), 0.0);
        float HdotV = max(dot(vHalfVec, vLook), 0.0);
    
        float3 vMetalic = lerp(float3(0.04f, 0.04f, 0.04f), vDiffuseColor.xyz, fMetalness);

        float NDF = trowbridgeReitzNDF(NdotH, fRoughness);
        float3 F = fresnel(vMetalic, HdotV, fRoughness);
        float G = schlickBeckmannGAF(NdotV, fRoughness) * schlickBeckmannGAF(NdotL, fRoughness);

        vector kS = vector(F, 0.f);
        vector kD = vector(1.f, 1.f, 1.f, 0.f) - kS;
        kD *= 1.f - fMetalness;

        vector vNumerator = kS * NDF * G;
        float fDenominator = 4.f * NdotV * NdotL;
        vector vSpecular = vNumerator / max(fDenominator, 0.001f);

        vector vSpecularAcc = vSpecular * NdotL * g_vLightDiffuse * fAtt;
        vector vAmbientColor = kD * vDiffuseColor / 3.141592265359 * fOcclusion * NdotL * g_vLightDiffuse * fAtt;


        Out.vSpecular = vSpecularAcc;
        Out.vSpecular.a = 0.f;
        
        Out.vAmbient = vAmbientColor;
        Out.vAmbient.a = 1.f;
      
    }
    else
    {
    //Out.vShade = vShadeDesc;
        vector vResult = g_vLightDiffuse * saturate(saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;

        if (vResult.r < 0.05f && vResult.g < 0.05f && vResult.b < 0.05f)
            discard;

        Out.vAmbient = vResult * fAtt;
        Out.vAmbient.a = 1.f;

        vector vReflect = reflect(normalize(g_vLightDir), vNormal);
   
        Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 20.f) * fAtt;
        Out.vSpecular.a = 0.f;
    }
    
    return Out;
}

PS_OUT PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT Out = (PS_OUT)Out;

	vector vDiffuse       = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    vector vNormalDesc    = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector vSpecular      = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
    vector vViewShadow    = g_ViewShadow.Sample(DefaultSampler, In.vTexUV);
    vector vDepthDesc     = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
    vector vFogDesc       = g_FogTexture.Sample(DefaultSampler, In.vTexUV);
    vector vORMDesc       = g_ORMTexture.Sample(DefaultSampler, In.vTexUV);
    vector vAmbientDesc   = g_AmbientTexture.Sample(DefaultSampler, In.vTexUV);
    vector vHBAO          = g_HBAOTexture.Sample(DefaultSampler, In.vTexUV);
    
    float fViewZ = vDepthDesc.y * g_fFar;

    vector vWorldPos;

	/* 투영스페이스 상의 위치르 ㄹ구한다. */
	/* 뷰스페이스 상 * 투영행렬 / w 까지 위치를 구한다. */
    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.0f;

	/* 뷰스페이스 상 * 투영행렬까지 곱해놓은 위치를 구한다. */
    vWorldPos *= fViewZ;

	/* 뷰스페이스 상  위치를 구한다. */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드페이스 상  위치를 구한다. */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
        
    float fCamDistance = length(vWorldPos - g_vCamPosition);
    
    bool bIsInShadow = 0.9f > vViewShadow.r;
  
    if (vORMDesc.r > 0.f || vORMDesc.g > 0.f || vORMDesc.b > 0.f)
    {
        vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
        vNormal = normalize(vNormal);
        vector vLook = normalize(g_vCamPosition - vWorldPos);
    
        float NdotV = max(dot(vNormal, vLook), 0.0);
    
        float fRoughness = max(0.01f, vORMDesc.y);
        float fMetalness = max(0.01f, vORMDesc.z);
        float fOcclusion = max(0.01f, vORMDesc.x);
     
        vector vReflect = normalize(reflect(-normalize(vLook), vNormal));
        
        float4 brdfTerm = g_BRDFTexture.Sample(ClampSampler, float2(NdotV, 1.0 - fRoughness));
        float3 metalSpecularIBL = g_PreFilterTexture.SampleLevel(ClampSampler, vReflect.xyz, (fRoughness * 10.f)).rgb;

        float3 dielectricColor = float3(0.04, 0.04, 0.04);
        float3 diffColor = vDiffuse.rgb * (1.0 - fMetalness);
        float3 specColor = lerp(dielectricColor.rgb, vDiffuse.rgb, fMetalness);
   
        vector diffuseIBL = g_IrradianceTexture.Sample(ClampSampler, vNormal) / 3.14159;
        diffuseIBL.rgb = lerp(diffuseIBL.rgb * 0.3f, diffuseIBL.rgb, fOcclusion);
    
        float3 albedoByDiffuse = diffColor.rgb * diffuseIBL.rgb;

        float4 litColor;
        litColor.rgb = (albedoByDiffuse.rgb + (metalSpecularIBL * (specColor * brdfTerm.x + (brdfTerm.y)))) * fOcclusion;
 
        litColor = pow(litColor, 2.2f);
      
        Out.vColor = vAmbientDesc + vSpecular*vORMDesc.a + (litColor * float4(g_IrradianceColorScale, 1.f));
        Out.vColor.rgb *= vViewShadow.rgb;
        Out.vColor.rgb *= vHBAO.rgb;
           
        Out.vColor.a = 1.f;
    }      
    else
    {
                                           
        Out.vColor = vDiffuse * vAmbientDesc + vSpecular /** (1.f - bIsInShadow)*/;
        Out.vColor.rgb *= vViewShadow.rgb;
    }
       

    
    float3 mapped = 1.f - exp(-Out.vColor.rgb * g_fExposure);
    Out.vColor.rgb = pow(mapped, 1.f / 2.2f);
    
   
        
    float fRatio = 1.f - exp(-fCamDistance / (g_fFogRange * 0.2f));
        
    Out.vColor.rgb = lerp(Out.vColor.rgb, float3(0.f, 0.f, 0.f), fRatio);
        
    if (0.01f < Out.vColor.a)
    {
        Out.vColor.rgb = (1.f - vFogDesc.r) * Out.vColor.rgb + vFogDesc.r * g_vFogColor.rgb;
    }
    else
    {
        Out.vColor.rgb = float3(0.f, 0.f, 0.f);
        Out.vColor.rgb = (1.f - vFogDesc.r) * Out.vColor.rgb + vFogDesc.r * g_vFogColor.rgb;
    }
    Out.vColor.a = 1.f;
      
    //if (vLightFlagDesc.r > 0.f || vLightFlagDesc.g > 0.f)
    //    return Out;

    //if (Out.vColor.a < 0.1f)
    //    discard;

	return Out;
}

//4
PS_OUT PS_MAIN_POSTEFFECT_MASK(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    vector vColor = vector(0.f, 0.f, 0.f, 0.f);

    float fBlurStart = 1.f;
    float2 center = float2(0.5f, 0.5f); //중심점<-마우스의 위치를 받아오면 마우스를 중심으로 블러됨

    In.vTexUV.xy -= center;

    float fPrecompute = g_fBlurWidth * (1.0f / 19.f);

    for (uint i = 0; i < 20; ++i)
    {
        float scale = fBlurStart + (float(i) * fPrecompute);
        float2 uv = In.vTexUV.xy * scale + center;

        vColor += g_BloomTexture.Sample(DefaultSampler, uv);
    }

    vColor /= 20.f;
	
    Out.vColor = vColor;
    //Out.vColor.a = g_PostEffectMaskTexture.Sample(DefaultSampler, In.vTexUV.xy).r;
    return Out;
}

//5
PS_OUT PS_MAIN_POSTEFFECT_BLOOM(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;
	
    //float fPower = 2.2f;
    //
    //Out.vColor = float4(1.f, 1.f, 1.f, 1.f);
	
    //float4 vHDRColor    = g_XBlurTexture.Sample(DefaultSampler, In.vTexUV.xy);
    //float4 vBloomColor  = g_ExtractBloomTexture.Sample(DefaultSampler, In.vTexUV.xy);
    //float4 vBloomOriTex = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV.xy);
     //vBloomOriTex.a = 1.f;

    //float4 vBloom = pow(pow(abs(vBloomColor), fPower) + pow(abs(vBloomOriTex), fPower), 1.f / fPower);
   

    //Out.vColor = pow(abs(vHDRColor), fPower);
    //vBloom = pow(abs(vBloom), fPower);

    //Out.vColor += vBloom;

    // Out.vColor = pow(abs(Out.vColor), 1 / fPower);


    // ---------------------------------------------------------------------------------
	//const float gamma = 2.2;
    //   float4 BlurTexture = g_XBlurTexture.Sample(DefaultSampler, In.vTexUV.xy);
    //   float3 hdrColor = BlurTexture.rgb;
    //   float fStrength = 0.2f;

    //// exposure tone mapping
    //float3 mapped = float3(1.f, 1.f, 1.f) - exp(-hdrColor * fStrength);
    //// gamma correction 
    //mapped = pow(mapped, (1.0 / gamma));

    //   Out.vColor = float4(mapped, BlurTexture.a);

    // ------------------------------------------------------------------------------------
    vector vShaderFlag = g_ShaderFlagTexture.Sample(DefaultSampler, In.vTexUV.xy);
    vector vBloomColor = 0;
   
    if(0.5f < vShaderFlag.b)
        vBloomColor= g_ExtractBloomTexture.Sample(DefaultSampler, In.vTexUV.xy);
    
    Out.vColor = g_XBlurTexture.Sample(DefaultSampler, In.vTexUV.xy) + vBloomColor;

    // Out.vColor *= g_vIntensity;

    //Out.vColor.a = vBloomOriTex.a;

    return Out;
}

PS_OUT PS_MAIN_GLOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;
	
    //Out.vColor = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV);
    //Out.vColor.a = 1.f;
	
    //Out.vColor.rgba = 1.f;
	
    vector OriginalEffect = g_OriginalEffectTexture.Sample(DefaultSampler, In.vTexUV);
	
    Out.vColor = g_GlowTexture.Sample(DefaultSampler, In.vTexUV);
    //Out.vColor.a = sqrt(Out.vColor.a);
	
    if (OriginalEffect.a > 0.05f)
        Out.vColor = OriginalEffect;
    
    //Out.vColor *= 1.5f; //Intensity;
	
    return Out;
}

PS_OUT PS_MAIN_COPY_ORIGIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;
    Out.vColor = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vColor.a = 1.f;
    return Out;
}

PS_OUT PS_MAIN_UP_GLOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;
	
    vector vUpGlowDesc = g_GlowTexture.Sample(DefaultSampler, In.vTexUV);
	
    Out.vColor = g_OriginalEffectTexture.Sample(DefaultSampler, In.vTexUV);
	
    if (vUpGlowDesc.a > 0.05f)
        Out.vColor = vUpGlowDesc;
	
    return Out;
}



PS_OUT PS_MAIN_VIEW_SHADOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT)1;
	
    vector vDepth = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
    float fViewZ  = vDepth.y * g_fFar;
    
    vector vWorldPos;
    /* 투영스페이스 상의 위치르 ㄹ구한다. */
	/* 뷰스페이스 상 * 투영행렬 / w 까지 위치를 구한다. */
    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepth.x;
    vWorldPos.w = 1.0f;

	/* 뷰스페이스 상 * 투영행렬까지 곱해놓은 위치를 구한다. */
    vWorldPos *= fViewZ;

	/* 뷰스페이스 상  위치를 구한다. */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드페이스 상  위치를 구한다. */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
    vector vStaticPosition = vWorldPos;                                                                            
    vStaticPosition = mul(vStaticPosition, g_LightViewMatrix);
	
    vStaticPosition = mul(vStaticPosition, g_LightProjMatrix);
    float2 vNewUV;
	
    vNewUV.x = (vStaticPosition.x / vStaticPosition.w) * 0.5f + 0.5f;
    vNewUV.y = (vStaticPosition.y / vStaticPosition.w) * -0.5f + 0.5f;
	
    vector vStaticShadowDepth = g_StaticShadowDepthTexture.Sample(ClampSampler, vNewUV);
    
    vector vDynamicPosition = vWorldPos;
    vDynamicPosition = mul(vDynamicPosition, g_DynamicLightViewMatrix);
	
    vDynamicPosition = mul(vDynamicPosition, g_DynamicLightProjMatrix);
    
    vNewUV.x = (vDynamicPosition.x / vDynamicPosition.w) * 0.5f + 0.5f;
    vNewUV.y = (vDynamicPosition.y / vDynamicPosition.w) * -0.5f + 0.5f;
    
    vector vShadowDepth = g_ShadowDepthTexture.Sample(ClampSampler, vNewUV);

    //Out.vColor = float4(vDynamicPosition.z / vDynamicPosition.w, vShadowDepth.r, 0.f, 1.f);
     
    // TODO : Hong Hong Hong Juseok
    if (vDynamicPosition.z / vDynamicPosition.w > vShadowDepth.r||
        vStaticPosition.z / vStaticPosition.w > vStaticShadowDepth.r)
        Out.vColor = 0.8f;

    return Out;
}

struct PS_OUT_FOG
{
    vector vFog: SV_TARGET0;
};


PS_OUT_FOG PS_MAIN_FOG(PS_IN In)
{
    PS_OUT_FOG		Out = (PS_OUT_FOG)1;

    /* 방향성광원의 정보와 노멀 타겟에 담겨있는 노멀과의 빛연산을 수행한다. */
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

    float fViewZ = vDepthDesc.y * g_fFar;
    /* 0 -> -1, 1 -> 1*/

    vector vWorldPos;

    /* 투영스페이스 상의 위치르 ㄹ구한다. */
    /* 뷰스페이스 상 * 투영행렬 / w 까지 위치를 구한다. */
    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.0f;

    /* 뷰스페이스 상 * 투영행렬까지 곱해놓은 위치를 구한다. */
    vWorldPos *= fViewZ;

    /* 뷰스페이스 상  위치를 구한다. */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

    /* 월드페이스 상  위치를 구한다. */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    

    vector vFogDir = vWorldPos - g_vCamPosition;

    float fDistance = length(vFogDir);

    //float			fAtt = saturate((g_fRange - fDistance) / g_fRange);
    float fAtt = saturate((g_fFogRange - fDistance) / g_fFogRange)  ;
    
    Out.vFog = (g_vFogColor.a - (fAtt * fAtt));
    

   // Out.vShade.a = 1.f;
    return Out;
}

PS_OUT PS_MAIN_SSR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
    vNormal = normalize(vector(vNormal.xyz * 2.f - 1.f, 0.f));
   
    vector vDepth = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
    
    float fViewZ = vDepth.y * g_fFar;
    
    vector vWorldPos;

    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepth.x;
    vWorldPos.w = 1.0f;

    vWorldPos *= fViewZ;

    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vViewDir = normalize(vWorldPos - g_vCamPosition);
    vViewDir.w = 0.f;
    
    vector vRayOrigin = vWorldPos;
    vector vRayDir = normalize(reflect(vViewDir, vNormal));
    vRayDir.w = 0.f;
    
    float fStep = 0.01f;
    
    matrix matVP = mul(g_CamViewMatrix, g_CamProjMatrix);
    
    float fPixelDepth = 0.f;
    int iStepDistance = 0;
    float2 vRayPixelPos = (float2) 0;
  
    for (iStepDistance = 1; iStepDistance < 50; ++iStepDistance)
    {
        vector vDirStep = vRayDir * fStep * iStepDistance;
        vDirStep.w = 0.f;
        vector vRayWorldPos = vRayOrigin + vDirStep;

        vector vRayProjPos = mul(vRayWorldPos, matVP);
        vRayProjPos.x =  vRayProjPos.x/vRayProjPos.w;
        vRayProjPos.y =  vRayProjPos.y/vRayProjPos.w;
      
        vRayPixelPos = float2(vRayProjPos.x * 0.5f + 0.5f, vRayProjPos.y * -0.5f + 0.5f);
        
        clip(vRayPixelPos);
        clip(1.f - vRayPixelPos);
        
        vector vPixelDepth = g_DepthTexture.Sample(DefaultSampler, vRayPixelPos);
        
        fPixelDepth = vPixelDepth.x;
        fPixelDepth *= vPixelDepth.y * g_fFar;
        
        float fDiff = vRayProjPos.z - fPixelDepth;
        
        if (fDiff > 0.f && fDiff < 0.01f)
            break;
    }
 
    clip(49.5f - iStepDistance);

    Out.vColor = g_OriginalRenderTexture.Sample(DefaultSampler, vRayPixelPos) * (1.f - iStepDistance / 50.f);
   
    return Out;
}

PS_OUT PS_MAIN_RIMLIGHT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vShaderflag = g_ShaderFlagTexture.Sample(DefaultSampler, In.vTexUV);
    clip(vShaderflag.a - 1.f);
   
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
    vector vRimDesc = g_RimLightTexture.Sample(DefaultSampler, In.vTexUV);
    vector vNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV) * 2.f -1.f;
    vNormal = normalize(vNormal);
    
    
    float fViewZ = vDepthDesc.y * g_fFar;
    /* 0 -> -1, 1 -> 1*/

    vector vWorldPos;

    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.0f;

    vWorldPos *= fViewZ;

    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vView = normalize(g_vCamPosition - vWorldPos);
    half fRim = 1.f - abs(dot(vNormal, vView));
    
    vector vRimColor = g_RimLightTexture.Sample(DefaultSampler, In.vTexUV);
    vRimColor.xyz = lerp(vRimColor.xyz*0.03f, vRimColor.xyz, fRim*fRim);
    
    
    half fRimPower = vRimColor.a;
      
    Out.vColor = vector(vRimColor.xyz, 1.f) * fRim * fRimPower;
    
    return Out;
}

BlendState BS_Default
{
	BlendEnable[0] = false;
};

BlendState BS_AlphaBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
    BlendEnable[2] = true;

	SrcBlend  = one;
	DestBlend = one;
	BlendOp   = add;

};

BlendState BS_ForwardAlphaBlend
{
    BlendEnable[0] = true;
    SrcBlend[0]    = SRC_ALPHA;
    DestBlend[0]   = INV_SRC_ALPHA;
    BlendOp[0]     = Add;
};


RasterizerState RS_Default
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

DepthStencilState DSS_Default
{
	DepthEnable    = true;
	DepthWriteMask = all;
	DepthFunc      = less_equal;
};

DepthStencilState DSS_ZEnable_ZWriteEnable_false
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

DepthStencilState DSS_DepthFalse_StencilEnable
{
    DepthEnable = false;
    DepthWriteMask = zero;

    StencilEnable = true;
    StencilReadMask = 0xff;
    StencilWriteMask = 0xff;

    FrontFaceStencilFunc = equal;
    FrontFaceStencilPass = keep;
    FrontFaceStencilFail = keep;
};

technique11 DefaultTechnique
{
	pass Debug //0
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_DEBUG();
	}

    pass Light_Directional //1
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

    pass Light_Point //2
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_LIGHT_POINT();
	}

    pass Blend //3
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_BLEND();
	}

    pass PostEffect_Mask //4
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_POSTEFFECT_MASK();
    }

    pass PostEffect_Bloom //5
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_POSTEFFECT_BLOOM();
    }

    pass Render_Glow //6
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_GLOW();
    }

    pass Copy_OriginalRenderTexture //7
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_COPY_ORIGIN();
    }

    pass Render_UpGlow //8
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_UP_GLOW();
    }

    pass Light_SpotLight //9
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_LIGHT_SPOTLIGHT();
    }

    pass View_Shadow //10
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_VIEW_SHADOW();
    }

    pass Fog//11
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_FOG();
    }

    pass SSR//12
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthFalse_StencilEnable, 2);
        SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SSR();
    }

    pass RimLight //13
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);
        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RIMLIGHT();
    }
}