
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_InvWorldMatrix;
matrix g_ViewMatrix, g_ProjMatrix;
matrix g_InvViewMatrix, g_InvProjMatrix;

float g_fAlphaValue = 1.f;
   
vector g_vColor = vector(0.f,1.f,0.7f,1.f);
      
texture2D	g_DiffuseTexture;
texture2D	g_NormalTexture;
texture2D	g_ORMTexture;
texture2D   g_EmissiveTexture1;
texture2D   g_EmissiveTexture2;

texture2D	g_DepthTexture;

float g_fFar = 300.f;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vTexUV : TEXCOORD0;
    float4		vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN_DECAL(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;

	return Out;	
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vTexUV : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct PS_OUT
{	
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    vector vORM : SV_Target2;
    vector vShaderFlag : SV_Target3;
    vector vExtractBloom : SV_Target4;
};

PS_OUT PS_MAIN_DECAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float2 vNewUV;
    vNewUV.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vNewUV.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
	
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vNewUV);
	//월드 변환->데칼 상자 로컬 변환
    float fViewZ = vDepthDesc.y * g_fFar;
	
    vector vWorldPos;
    vWorldPos.x = vNewUV.x*2.f - 1.f;
    vWorldPos.y = vNewUV.y*-2.f +1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
    vWorldPos *= fViewZ;
    
    vWorldPos = mul(vWorldPos, g_InvProjMatrix);
    vWorldPos = mul(vWorldPos, g_InvViewMatrix);
    
    vector vDecalLocalPos = mul(vWorldPos, g_InvWorldMatrix);
    
    if (vDecalLocalPos.x > 0.5f || vDecalLocalPos.x < -0.5f ||
        vDecalLocalPos.y > 0.5f || vDecalLocalPos.y < -0.5f||
        vDecalLocalPos.z > 0.5f || vDecalLocalPos.z < -0.5f)
        discard;
    
    float2 vDecalUV = vDecalLocalPos.xy;
    vDecalUV.x = vDecalUV.x + 0.5f;
    vDecalUV.y = 0.5f - vDecalUV.y ;
    
    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, vDecalUV);
    Out.vDiffuse.a = 1.f;
    Out.vDiffuse.a *= g_fAlphaValue;
    clip(Out.vDiffuse.a - 0.01f);

    Out.vNormal = normalize(mul(vector(g_NormalTexture.Sample(DefaultSampler, vDecalUV).xyz, 0.f), g_WorldMatrix));
    Out.vNormal.a = 1.f;
    Out.vNormal.a*= g_fAlphaValue;
    Out.vNormal.xyz = Out.vNormal.xyz *0.5f + 0.5f;
          
    Out.vORM = g_ORMTexture.Sample(DefaultSampler, vDecalUV);
    Out.vORM.a *= g_fAlphaValue;
      
    Out.vShaderFlag = 0.f;
    Out.vShaderFlag.b = 0.f < g_EmissiveTexture1.Sample(DefaultSampler, vDecalUV).r + g_EmissiveTexture2.Sample(DefaultSampler, vDecalUV).r; //일단 보류 emissive넣고 싶은 곳 텍스처로 굽는게 나을듯
    Out.vExtractBloom = g_vColor;
    Out.vExtractBloom.a = (g_EmissiveTexture1.Sample(DefaultSampler, vDecalUV).r+ g_EmissiveTexture2.Sample(DefaultSampler, vDecalUV).r) * g_fAlphaValue;
	    
    return Out;
}
      
DepthStencilState DSS_DecalDepthStencil
{//스텐실 테스트는 깊이 테스트를 활성화 해야 됨
    DepthEnable = true;
    DepthWriteMask = zero;
    DepthFunc = always;

    StencilEnable = true;
    StencilReadMask = 0xff;
      
    BackFaceStencilFunc = equal;
    BackFaceStencilPass = keep;
    BackFaceStencilFail = keep;
};
BlendState BS_DecalBlending
{
    BlendEnable[0] = true;
    BlendEnable[1] = true;
    BlendEnable[2] = true;

    SrcBlend[0] = SRC_ALPHA;
    DestBlend[0] = INV_SRC_ALPHA;
    BlendOp[0] = add;

    SrcBlend[1] = SRC_ALPHA;
    DestBlend[1] = INV_SRC_ALPHA;
    BlendOp[1] = add;

    SrcBlend[2] = SRC_ALPHA;
    DestBlend[2] = INV_SRC_ALPHA;
    BlendOp[2] = add;

};

technique11 DefaultTechnique
{  
	pass Decal
	{      
        SetBlendState(BS_DecalBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DecalDepthStencil, 0);
        SetRasterizerState(RS_Sky);

        VertexShader = compile vs_5_0 VS_MAIN_DECAL();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DECAL();
    }
}