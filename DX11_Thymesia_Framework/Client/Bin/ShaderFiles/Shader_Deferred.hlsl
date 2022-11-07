
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix		g_ProjMatrixInv, g_ViewMatrixInv;
matrix		g_LightViewMatrix, g_LightProjMatrix;


// For Light Render
vector		g_vLightDir;

vector		g_vLightPos;
float		g_fRange; 

vector		g_vCamPosition;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;
vector		g_vLightFlag;

vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(0.1f, 0.1f, 0.1f, 1.f);


texture2D	g_SpecularTexture;
texture2D	g_DepthTexture;
texture2D	g_DiffuseTexture;
texture2D	g_ShadeTexture;
texture2D	g_NormalTexture;
texture2D	g_LightFlagTexture;
texture2D	g_ShadowDepthTexture;
texture2D	g_Texture;
texture2D   g_ViewShadow;



texture2D	g_XBlurTexture;
texture2D	g_ExtractBloomTexture;
texture2D	g_OriginalRenderTexture;

texture2D g_PostEffectMaskTexture;
texture2D g_BloomTexture;

texture2D g_GlowTexture;
texture2D g_OriginalEffectTexture;

// For Motion Blur
float		g_fBlurWidth;

float		g_fFar = 300;

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
};

sampler ClampSampler = sampler_state
{
    filter = min_mag_linear_mip_point;
    AddressU = clamp;
    AddressV = clamp;
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

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);	

	return Out;
}

struct PS_OUT_LIGHT
{
	vector		vShade : SV_TARGET0;
	vector		vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)1;

	/* 방향성광원의 정보와 노멀 타겟에 담겨있는 노멀과의 빛연산을 수행한다. */
	vector			vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
    vector			vLightFlagDesc = g_LightFlagTexture.Sample(DefaultSampler, In.vTexUV);
    //vector			vShadeDesc = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
	
	//결과가 0이라면 일치하는 라이트 플래그가 없다. 그리지 않는다.
    vLightFlagDesc *= g_vLightFlag;
	
    float fMaxScalar = 0.f;
	
    for (uint i = 0; i < 4; ++i)
    {
        fMaxScalar = max(fMaxScalar, vLightFlagDesc[i]);
    }
	
    if (fMaxScalar < 0.05f)
    {
        discard;
    }
	
    float fViewZ = vDepthDesc.y * 300.f;
	/* 0 -> -1, 1 -> 1*/
	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
	
    //Out.vShade = vShadeDesc;
	
    vector vResult = g_vLightDiffuse * saturate(saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient));
    
    if (vResult.r < 0.05f && vResult.g < 0.05f && vResult.b < 0.05f)
    {
        discard;
    }
    
    vector vCartoonShader = ceil(vResult * 8.f) / 8.f;
    
    Out.vShade = vCartoonShader;
    Out.vShade.a = 1.f;

	vector			vReflect = reflect(normalize(g_vLightDir), vNormal);

	vector			vWorldPos;

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

	vector			vLook = normalize(vWorldPos - g_vCamPosition);

    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 5.f);
	Out.vSpecular.a = 0.f;


	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)1;

	/* 방향성광원의 정보와 노멀 타겟에 담겨있는 노멀과의 빛연산을 수행한다. */
	vector			vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
    //vector			vShadeDesc = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
    vector          vLightFlagDesc = g_LightFlagTexture.Sample(DefaultSampler, In.vTexUV);
    
    vLightFlagDesc *= g_vLightFlag;
	
    float fMaxScalar = 0.f;
	
    for (uint i = 0; i < 4; ++i)
    {
        fMaxScalar = max(fMaxScalar, vLightFlagDesc[i]);
    }
	
    if (fMaxScalar < 0.05f)
    {
        discard;
    }
    
	float			fViewZ = vDepthDesc.y * 300.f;
	/* 0 -> -1, 1 -> 1*/
	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector			vWorldPos;

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

	vector			vLightDir = vWorldPos - g_vLightPos;

	float			fDistance = length(vLightDir);

	float			fAtt = saturate((g_fRange - fDistance) / g_fRange);

    //Out.vShade = vShadeDesc;
    vector vResult = g_vLightDiffuse * saturate(saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
    
    if (vResult.r < 0.05f && vResult.g < 0.05f && vResult.b < 0.05f)
    {
        discard;
    }
    
    if (vLightFlagDesc.z < 0.1f)
    {
        vResult = ceil(vResult * 8.f) / 8.f;
    }
    
    Out.vShade = vResult;
	Out.vShade.a = 1.f;

	vector			vReflect = reflect(normalize(vLightDir), vNormal);
	vector			vLook = normalize(vWorldPos - g_vCamPosition);

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 2.f) * fAtt;
	Out.vSpecular.a = 0.f;


	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_HALF_POINT(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 1;

	/* 방향성광원의 정보와 노멀 타겟에 담겨있는 노멀과의 빛연산을 수행한다. */
    vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
    //vector			vShadeDesc = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
    vector vLightFlagDesc = g_LightFlagTexture.Sample(DefaultSampler, In.vTexUV);
    
    vLightFlagDesc *= g_vLightFlag;
	
    float fMaxScalar = 0.f;
	
    for (uint i = 0; i < 4; ++i)
    {
        fMaxScalar = max(fMaxScalar, vLightFlagDesc[i]);
    }
	
    if (fMaxScalar < 0.05f)
    {
        discard;
    }
    
    float fViewZ = vDepthDesc.y * 300.f;
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
    
    float fLightDirAtt = dot(normalize(g_vLightDir), normalize(vLightDir));
    
    /* 내적 결과 1 ~ -1을 1 ~ 0으로 바꾼다. */ 
    fLightDirAtt = (fLightDirAtt + 1.f) * 0.5f;

    float fDistance = length(vLightDir);

    float fAtt = saturate((g_fRange - fDistance) / g_fRange);

    //Out.vShade = vShadeDesc;
    vector vResult = g_vLightDiffuse * saturate(saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt * fLightDirAtt;
    
    if (vResult.r < 0.05f && vResult.g < 0.05f && vResult.b < 0.05f)
    {
        discard;
    }
    
    // 지형이 아니라면 카툰 렌더링한다. 지형에 카툰렌더링 조명이 들어가면 어색해보인다.
    if (vLightFlagDesc.z < 0.1f)
    {
        vResult = ceil(vResult * 8.f) / 8.f;
    }
    
    Out.vShade = vResult;
    Out.vShade.a = 1.f;

    vector vReflect = reflect(normalize(vLightDir), vNormal);
    vector vLook = normalize(vWorldPos - g_vCamPosition);

    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 2.f) * fAtt;
    Out.vSpecular.a = 0.f;


    return Out;
}



PS_OUT PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)Out;

	vector			vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
    vector			vLightFlagDesc = g_LightFlagTexture.Sample(DefaultSampler, In.vTexUV);
    vector          vViewShadow = g_ViewShadow.Sample(DefaultSampler, In.vTexUV);

    Out.vColor = vDiffuse * vShade + vSpecular;
	
    if (vLightFlagDesc.r > 0.f || vLightFlagDesc.g > 0.f)
    {
        return Out;
    }
	
    Out.vColor.rgb *= vViewShadow.rgb;
    
    
    
 //   vector vDepth = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
 //   float fViewZ = vDepth.y * g_fFar;
    
 //   vector vWorldPos;
 //   /* 투영스페이스 상의 위치르 ㄹ구한다. */
	///* 뷰스페이스 상 * 투영행렬 / w 까지 위치를 구한다. */
 //   vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
 //   vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
 //   vWorldPos.z = vDepth.x;
 //   vWorldPos.w = 1.0f;

	///* 뷰스페이스 상 * 투영행렬까지 곱해놓은 위치를 구한다. */
 //   vWorldPos *= fViewZ;

	///* 뷰스페이스 상  위치를 구한다. */
 //   vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	///* 월드페이스 상  위치를 구한다. */
 //   vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
 //   vector vPosition = vWorldPos;
 //   vPosition = mul(vPosition, g_LightViewMatrix);
	
 //   vector vUVPos = mul(vPosition, g_LightProjMatrix);
 //   float2 vNewUV;
	
 //   vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
 //   vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;
	
 //   vector vShadowDepth = g_ShadowDepthTexture.Sample(ClampSampler, vNewUV);

 //   if (vPosition.z - 0.15f > vShadowDepth.r * g_fFar)
 //   {
 //       Out.vColor = Out.vColor * 0.4f;
		
 //   }
	
    if (Out.vColor.a == 0.f)
        discard;

	return Out;
}

//4
PS_OUT PS_MAIN_POSTEFFECT_MASK(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vColor = vector(0.f, 0.f, 0.f, 0.f);

    float fBlurStart = 1.f;
    float2 center = float2(0.5f, 0.5f); //중심점<-마우스의 위치를 받아오면 마우스를 중심으로 블러됨

    In.vTexUV.xy -= center;

    float fPrecompute = g_fBlurWidth * (1.0f / 19.f);

    for (uint i = 0; i < 20; ++i)
    {
        float scale = fBlurStart + (float(i) * fPrecompute);
        float2 uv = In.vTexUV.xy * scale + center;

        if (0.f > uv.x)
            uv.x = 0.f;
        else if (1.f < uv.x)
            uv.x = 1.f;

        if (0.f > uv.y)
            uv.y = 0.f;
        else if (1.f < uv.y)
            uv.y = 1.f;

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
    PS_OUT Out = (PS_OUT) 0;
	
    
    Out.vColor = float4(1.f, 1.f, 1.f, 1.f);
	
    float4 vHDRColor = g_XBlurTexture.Sample(DefaultSampler, In.vTexUV.xy);
    float4 vBloomColor = g_ExtractBloomTexture.Sample(DefaultSampler, In.vTexUV.xy);
    float4 vBloomOriTex = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV.xy);
    vBloomOriTex.a = 1.f;

    float4 vBloom = pow(pow(abs(vBloomColor), 2.2f) + pow(abs(vBloomOriTex), 2.2f), 1.f / 2.2f);

    Out.vColor = vHDRColor;
     
    Out.vColor = pow(abs(Out.vColor), 2.2f);
    vBloom = pow(abs(vBloom), 2.2f);

    Out.vColor += vBloom;

    Out.vColor = pow(abs(Out.vColor), 1 / 2.2f);
	
    return Out;
}

PS_OUT PS_MAIN_GLOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    //Out.vColor = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV);
    //Out.vColor.a = 1.f;
	
    //Out.vColor.rgba = 1.f;
	
    vector OriginalEffect = g_OriginalEffectTexture.Sample(DefaultSampler, In.vTexUV);
	
    Out.vColor = g_GlowTexture.Sample(DefaultSampler, In.vTexUV);
    //Out.vColor.a = sqrt(Out.vColor.a);
	
    if (OriginalEffect.a > 0.05f)
    {
        Out.vColor = OriginalEffect;

    }
	
    return Out;
}

PS_OUT PS_MAIN_COPY_ORIGIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV);
	
    return Out;
}

PS_OUT PS_MAIN_UP_GLOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    vector vUpGlowDesc = g_GlowTexture.Sample(DefaultSampler, In.vTexUV);
	
    Out.vColor = g_OriginalEffectTexture.Sample(DefaultSampler, In.vTexUV);
	
    if (vUpGlowDesc.a > 0.05f)
    {
        Out.vColor = vUpGlowDesc;

    }
	
    return Out;
}

PS_OUT PS_MAIN_VIEW_SHADOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 1;
	
    vector vDepth = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
    float fViewZ = vDepth.y * g_fFar;
    
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
	
    vector vPosition = vWorldPos;
    vPosition = mul(vPosition, g_LightViewMatrix);
	
    vector vUVPos = mul(vPosition, g_LightProjMatrix);
    float2 vNewUV;
	
    vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
    vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;
	
    vector vShadowDepth = g_ShadowDepthTexture.Sample(ClampSampler, vNewUV);

    if (vPosition.z - 0.15f > vShadowDepth.r * g_fFar)
    {
        Out.vColor = 0.4f;
		
    }

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

	SrcBlend = one;
	DestBlend = one;
	BlendOp = add;
};

BlendState BS_ForwardAlphaBlend
{
    BlendEnable[0] = true;
    SrcBlend[0] = SRC_ALPHA;
    DestBlend[0] = INV_SRC_ALPHA;
    BlendOp[0] = Add;
};

RasterizerState RS_Default
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_ZEnable_ZWriteEnable_false
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

technique11 DefaultTechnique
{
	pass Debug //0
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

    pass Light_Directional //1
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

    pass Light_Point //2
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
	}

    pass Blend //3
    {
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}

    pass PostEffect_Mask //4
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POSTEFFECT_MASK();
    }

    pass PostEffect_Bloom //5
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POSTEFFECT_BLOOM();
    }

    pass Render_Glow //6
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW();
    }

    pass Copy_OriginalRenderTexture //7
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_COPY_ORIGIN();
    }

    pass Render_UpGlow //8
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UP_GLOW();
    }

    pass Light_HalfPoint //9
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHT_HALF_POINT();
    }

    pass View_Shadow //10
    {
        SetBlendState(BS_ForwardAlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_VIEW_SHADOW();
    }
}