
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix,g_ViewMatrix, g_ProjMatrix; //직교투영

uint3 g_ChannelIndex;
texture2D g_SourTexture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN_DECAL(VS_IN In)
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
    float4 vPosition : SV_POSITION;
    float3 vTexUV : TEXCOORD0;
};

struct PS_OUT
{	
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DECAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    //vector vTextureDesc = g_SourTexture.Sample(DefaultSampler, In.vTexUV);
    vector vTextureDesc = g_SourTexture.Sample(sRGBSampler, In.vTexUV);
    
    Out.vColor.r = vTextureDesc[g_ChannelIndex.r];
    Out.vColor.g = vTextureDesc[g_ChannelIndex.g];
    Out.vColor.b = vTextureDesc[g_ChannelIndex.b];  
       
    Out.vColor.rgb = pow(Out.vColor.rgb, 2.2f);          
    
    Out.vColor.a = vTextureDesc.a;
   
    //Out.vColor = vTextureDesc;
    return Out;
}
      


technique11 DefaultTechnique
{  
	pass Decal
	{      
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_DECAL();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DECAL();
    }
}