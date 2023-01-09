
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix     , g_ViewMatrix	, g_ProjMatrix; //직교투영
matrix g_ViewMatrixInv	  , g_ProjMatrixInv; //월드 <-> 스크린

matrix g_PreCamViewMatrix, g_CamProjMatrix,g_CamViewMatrix; //proj는 변하지 않는다고 가정

vector		g_vCamPosition;
vector g_vLightPos;
vector g_vLightDiffuse;
float g_fGodRayScale;

float g_fFar = 300;

float g_PixelWidth;
float g_PixelHeight;

vector		g_vLinearVelocity;
vector		g_vAngularVelocity;

texture2D	g_DepthTexture;
texture2D	g_OriginalRenderTexture;
//Chromatic
float		g_fChromaticStrength = 0.f;

//MotionBlur
float		g_fMotionBlurStrength = 0.f;
//RadialBlur
float3		g_vBlurWorldPosition;
float		g_fRadialBlurStrength;

//liftgammaain
vector g_vLift, g_vGamma, g_vGain;
//ScreenTone
float g_fGrayScale;
float g_fContrastValue;
float g_fSaturation;

//Color Inversion
float g_fInversionStrength;
float g_fInversionRatio;

texture2D g_MaskTexture;
texture2D g_StaticShadowDepthTexture;

static const float BlurWeights[13] =
{
	0.002216f,
	0.008764f,
	0.026995f,
	0.064759f,
	0.120985f,
	0.176033f,
	0.199471f,
	0.176033f,
	0.120985f,
	0.064759f,
	0.026995f,
	0.008764f,
	0.002216f,
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

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV    = In.vTexUV;

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

PS_OUT PS_MAIN_GODRAY(PS_IN In)
{
    clip(g_vLightDiffuse.a - 0.1f);
    clip(g_fGodRayScale - 0.01f);
 
    float fDistance = length(g_vLightPos - g_vCamPosition);
    clip(fDistance - 5.f);
    
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV);
    
    //if(0.1f >g_vLightDiffuse.a || 0.01f > g_fGrayScale)
    //    return Out;
        
    matrix matVP = mul(g_CamViewMatrix, g_CamProjMatrix);
       
    float4 vScreenLightPos = mul(g_vLightPos, matVP);
    
    clip(vScreenLightPos.z);
    
    vScreenLightPos /= vScreenLightPos.w;
    vScreenLightPos.x = vScreenLightPos.x * 0.5f + 0.5f;
    vScreenLightPos.y = vScreenLightPos.y * -0.5f + 0.5f;
    
    
    half2 vDeltaTexCoord = In.vTexUV - vScreenLightPos.xy;
    half fDither = g_MaskTexture.Sample(DefaultSampler, In.vTexUV).r;
    vDeltaTexCoord *= 1.0f / 64.f /*NUM_SAMPLES*/;
      
    half illuminationDecay = 1.0f;
    float s = 0.f;
    
    float2 vSampleUV = In.vTexUV;
    
    for (int i = 0; i < 64; ++i)
    {
        vSampleUV -= vDeltaTexCoord;
        
        float fDepth = g_DepthTexture.Sample(DefaultSampler, vSampleUV + vDeltaTexCoord * fDither).x;
  
        s += (fDepth >= 0.9999f) * 1.f / 64.f;
    }
    
    float fAtt = min(0.3f, max(0.01f, 10.f / fDistance));
    
    Out.vColor = lerp(Out.vColor, g_vLightDiffuse, fAtt * s * g_fGodRayScale);
    
    return Out;
}

PS_OUT PS_MAIN_COLOR_INVERSION(PS_IN In)
{
    clip(1.f - g_fInversionRatio);
    
    PS_OUT Out = (PS_OUT) 0;
    
    float3 vColor = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV).xyz;
    
    float3 vInversedColor = (1.f - vColor) * g_fInversionStrength;
    
    Out.vColor.rgb = lerp(vInversedColor, vColor, g_fInversionRatio);
    Out.vColor.a = 1.f;
    
    return Out;
}

PS_OUT PS_MAIN_MOTION_BLUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
		   
	float fViewZ = vDepthDesc.y * g_fFar;
		    
	vector vPixelWorldPos,vPixelPos;

	vPixelWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vPixelWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vPixelWorldPos.z = vDepthDesc.x;
	vPixelWorldPos.w = 1.0f;

	vPixelPos = vPixelWorldPos;
	    
	vPixelWorldPos *= fViewZ;

	vPixelWorldPos = mul(vPixelWorldPos, g_ProjMatrixInv);

	vPixelWorldPos = mul(vPixelWorldPos, g_ViewMatrixInv);
	
	matrix matVP   = mul(g_PreCamViewMatrix, g_CamProjMatrix);

	vector vPrePixelPos = mul(vPixelWorldPos, matVP);
	vPrePixelPos /= vPrePixelPos.w;

	float2 vPixelVelocity = ((vPixelPos - vPrePixelPos) * 0.5f).xy;
	float2 texCoord       = In.vTexUV;

	vector vColor = vector(0.f,0.f,0.f,0.f);

	for (int i = -10; i < 10; ++i)
	{
        texCoord += vPixelVelocity * (/*0.005f + */g_fMotionBlurStrength) * i;
		float4 currentColor = g_OriginalRenderTexture.Sample(ClampSampler, texCoord);
		vColor += currentColor;
	}

	Out.vColor = vColor / 20.f;
	
	return Out;
}

PS_OUT PS_MAIN_CHROMATIC(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 center = float2(0.5f, 0.5f);

	float2 BlurDir = In.vTexUV - center;

	vector vColor;

	vColor.r = g_OriginalRenderTexture.Sample(ClampSampler, In.vTexUV).r;
    vColor.g = g_OriginalRenderTexture.Sample(ClampSampler, In.vTexUV - BlurDir * g_fChromaticStrength * 0.5f).g;
    vColor.b = g_OriginalRenderTexture.Sample(ClampSampler, In.vTexUV - BlurDir * g_fChromaticStrength).b;

	//vColor *= (1.0 - g_BlurStrength * 0.5); //안해주면 그냥 밝아짐

	Out.vColor = vColor;

	return Out;
}

PS_OUT PS_MAIN_LIFTGAMMAGAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vColor = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV);
	
    vColor *= 1.1f;
	
	//Lift
    vector vLift = g_vLift;
    vColor = vColor * (1.5f - 0.5f * vLift) + 0.5f * vLift - 0.5f;
    vColor = saturate(vColor); //isn't strictly necessary, but doesn't cost performance.
	
	//Gain = 1.f <- 임시
	//vector vGain = vector(1.f,1.f,1.f,1.f,);
    vColor *= g_vGain;
	
	                              //Gamma =1.3f <- 임시
    vector vGamma = g_vGamma;
    Out.vColor = pow(vColor, 1.0 / vGamma); //Gamma

    return Out;
}

PS_OUT PS_MAIN_SCREENTONE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    vector vColor = g_OriginalRenderTexture.Sample(DefaultSampler, In.vTexUV);
	
    vColor *= g_fSaturation;
	
    float avg = (vColor.r + vColor.g + vColor.b) / 3.0;
    vector vNewColor;
    vNewColor.a = 1.0f;
    vNewColor.rgb = avg * (1.0 - g_fGrayScale) + vColor.rgb * g_fGrayScale;
  

    Out.vColor = 0.5f + g_fContrastValue * (vNewColor - 0.5f);
    
	
    //Out.vColor = vNewColor;
	
    return Out;
}


PS_OUT PS_MAIN_RADIALBLUR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vColor = vector(0.f, 0.f, 0.f, 0.f);

    float fBlurStart = 1.f;
	
    matrix matVP = mul(g_CamViewMatrix, g_CamProjMatrix);
    vector vBlurCenter = mul(vector(g_vBlurWorldPosition, 1.f), matVP);
    vBlurCenter /= vBlurCenter.w;

    vBlurCenter.x = vBlurCenter.x *  0.5f + 0.5f;
    vBlurCenter.y = vBlurCenter.y * -0.5f + 0.5f;
	
    float2 center = float2(vBlurCenter.x, vBlurCenter.y); //중심점<-마우스의 위치를 받아오면 마우스를 중심으로 블러됨
	
    In.vTexUV.xy -= center;

    float fPrecompute = g_fRadialBlurStrength * (1.0f / 19.f);
    int iDivision = 0;
	
    for (uint i = 0; i < 20; ++i)
    {
        float scale = fBlurStart + (float(i) * fPrecompute);
        float2 uv = In.vTexUV.xy * scale + center;
		
        if (0.f > uv.x || 1.f < uv.x)
            continue;
		
        if (0.f > uv.y || 1.f < uv.y)
            continue;


        vColor += g_OriginalRenderTexture.Sample(ClampSampler, uv);
        ++iDivision;
    }

    vColor /= (float) iDivision;
	
    //float2 vBlurDir = In.vTexUV.xy - center;
	
    //for (int i = 0; i < 10; ++i)
    //{
    //    float4 currentColor = g_OriginalRenderTexture.Sample(ClampSampler, In.vTexUV + vBlurDir * g_fRadialBlurStrength*0.05f * i);
    //    vColor += currentColor;
    //}
	
    Out.vColor = vColor;
	
    return Out;
}


DepthStencilState DSS_None_ZTestWrite_True_StencilTest
{
    DepthEnable = false;
    DepthWriteMask = zero;

    StencilEnable = true;
    StencilReadMask = 0xff;
	
    FrontFaceStencilFunc = not_equal;
    FrontFaceStencilPass = keep;
    FrontFaceStencilFail = keep;
};


technique11 DefaultTechnique
{
    pass God_Ray//0
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GODRAY();
    }
    pass Color_Inversion//1
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_COLOR_INVERSION();
    }

    pass Lift_Gamma_Gain //2
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIFTGAMMAGAIN();
    }
    pass ScreenTone //3
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SCREENTONE();
    }
	pass Chromatic_Aberration//4
	{
		SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_CHROMATIC();
	}
	pass MotionBlur //5
	{
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
		SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader    = compile ps_5_0 PS_MAIN_MOTION_BLUR();
	}

    pass RadialBlur//6
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTestWrite_True_StencilTest, 1);
       // SetDepthStencilState(DSS_None_ZTest_And_Write, 0);    
		SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RADIALBLUR();
    }
}