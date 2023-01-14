
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, g_RotationMatrix;
matrix	g_Points;

float2 g_vUVMask;
float fWrapWeight;

texture2D	g_MaskTexture1;
texture2D   g_MaskTexture2;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE0;
    float2		vRatio : PSIZE1;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE0;
    float2		vRatio : PSIZE1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	//matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	/* 로컬스페이스에 존재하낟. */
	//vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	//Out.vPosition = mul(vPosition, g_WorldMatrix);

	float3			vPSize = float3(In.vPSize.x, In.vPSize.y, 1.f);

    Out.vPosition = float4(In.vPosition, 1.f);
    Out.vPSize = In.vPSize;
    Out.vRatio = In.vRatio;
	
	
	return Out;	
}


float3 bezier(float3 P0, float3 P1, float3 P2, float3 P3, float Ratio)
{
    float t = Ratio;
    float t2 = t * t;
    float one_minus_t = 1.0 - t;
    float one_minus_t2 = one_minus_t * one_minus_t;
    return (P0 * one_minus_t2 * one_minus_t + P1 * 3.0 * t * one_minus_t2 + P2 * 3.0 * t2 * one_minus_t + P3 * t2 * t);
}

struct GS_IN
{
	float3		vPosition : POSITION;
    float2 vPSize : PSIZE0;
    float2 vRatio : PSIZE1;
};

struct GS_OUT
{
	vector		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

/* 지오메트리 셰이더. */

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> Stream)
{
	GS_OUT			Out[4];
    float fStrangth = 0.03f;
	
    float3 vLook = normalize(g_Points[2].xyz - g_Points[1].xyz);
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
	
    float3 vPosition = bezier(g_Points[0].xyz, g_Points[1].xyz, g_Points[2].xyz, g_Points[3].xyz, In[0].vRatio.x);
    float3 vNextPosition = bezier(g_Points[0].xyz, g_Points[1].xyz, g_Points[2].xyz, g_Points[3].xyz, In[0].vRatio.y);

    //vPosition = mul(float4(vPosition, 1.f), g_RotationMatrix).xyz;
    //vNextPosition = mul(float4(vNextPosition, 1.f), g_RotationMatrix).xyz;
	
    Out[0].vPosition = float4(vPosition + (vRight * In[0].vPSize.x * fStrangth), 1.f);
    Out[1].vPosition = float4(vPosition + (vRight * -In[0].vPSize.x * fStrangth), 1.f);
	
    Out[2].vPosition = float4(vNextPosition + (vRight * In[0].vPSize.x * fStrangth), 1.f);
    Out[3].vPosition = float4(vNextPosition + (vRight * -In[0].vPSize.x * fStrangth), 1.f);
	
    matrix matVP;

	matVP = mul(g_ViewMatrix, g_ProjMatrix);
	
    //Out[0].vPosition = mul(Out[0].vPosition, g_RotationMatrix);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexUV = float2(In[0].vRatio.x, 0.f);
	
    //Out[1].vPosition = mul(Out[1].vPosition, g_RotationMatrix);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexUV = float2(In[0].vRatio.x, 1.f);
	
    //Out[2].vPosition = mul(Out[2].vPosition, g_RotationMatrix);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexUV = float2(In[0].vRatio.y, 0.f);
	
    //Out[3].vPosition = mul(Out[3].vPosition, g_RotationMatrix);
    Out[3].vPosition = mul(Out[3].vPosition, matVP) ;
    Out[3].vTexUV = float2(In[0].vRatio.y, 1.f);
	
    Stream.Append(Out[2]);
    Stream.Append(Out[3]);
    Stream.Append(Out[1]);
    Stream.RestartStrip();

    Stream.Append(Out[2]);
    Stream.Append(Out[1]);
    Stream.Append(Out[0]);
    Stream.RestartStrip();
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

    Out.vColor = 1.f;

    float fMaskDesc = g_MaskTexture1.Sample(DefaultSampler, In.vTexUV - g_vUVMask).r*0.2f;
    //clip(fMaskDesc - 0.01f);
      
    half2 vTexUV = In.vTexUV;
    vTexUV.x *= fWrapWeight*0.8f;
 
    Out.vColor.a *= (g_MaskTexture2.Sample(DefaultSampler, vTexUV - g_vUVMask).r * 0.4f + fMaskDesc);
    Out.vColor.a *= Out.vColor.a;
    Out.vColor.a = max(0.01f, Out.vColor.a);
    
    
	return Out;	
}

technique11 DefaultTechnique
{
	pass Default
	{
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZTest_And_No_Write, 0);
		SetRasterizerState(RS_NonCulling);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}