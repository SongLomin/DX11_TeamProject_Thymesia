#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float g_AhlpaScale;
texture2D	g_DiffuseTexture;
texture2D   g_MaskTexture;
float g_Ratio, g_RedRatio;
float4 g_DefaultColor;

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
	
    //float4 Ahlpa = g_DiffuseTexture.GatherAlpha(DefaultSampler, In.vTexUV);
    //Out.vColor.a = Ahlpa.r;

	return Out;
}

PS_OUT PS_MAIN_MONSTER_HPBAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 fReverseTexUV = In.vTexUV.yx;
    
    float4 vMaskColor = g_MaskTexture.Sample(DefaultSampler, fReverseTexUV);
    
    Out.vColor.rgb = vMaskColor.rgb;
    Out.vColor.a = 1.f;
	
    float fPartCheck = In.vTexUV.x - g_Ratio;
    
    if (fPartCheck < 0.f)
    {
        Out.vColor *= float4(1.f, 1.f, 1.f, 1.f);
        return Out;
    }
    
    fPartCheck -= g_RedRatio;
    
    if (fPartCheck < 0.f)
    {
        Out.vColor *= float4(1.f, 0.2f, 0.2f, 1.f);
        return Out;
    }
    
    Out.vColor *= g_DefaultColor;
    return Out;
}

PS_OUT PS_MAIN_PLAYER_HPBAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    //Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    //Out.vColor.a *= g_AhlpaScale;
	
    float Color = sign(g_Ratio - In.vTexUV.x);
    Color = max(0, Color);

    Out.vColor = float4(Color, Color, Color, 1.f);
	
    float2 fReverseTexUV = In.vTexUV.yx;
    
    float4 vMaskColor = g_MaskTexture.Sample(DefaultSampler, fReverseTexUV);
    
    //Out.vColor.r *= vMaskColor.r;
    
    Out.vColor.rgb *= vMaskColor.rgb;
    
    //float4 Ahlpa = g_DiffuseTexture.GatherAlpha(DefaultSampler, In.vTexUV);
    //Out.vColor.a = Ahlpa.r;

    return Out;
}

PS_OUT PS_MAIN_COMBO_BAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    //Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    //Out.vColor.a *= g_AhlpaScale;
	
    float Color = sign(g_Ratio - In.vTexUV.x);
    Color = max(0, Color);

    Out.vColor = float4(Color, Color, Color, 1.f);
    
    if(Out.vColor.r < 0.01f)
        discard;
    
    //float4 Ahlpa = g_DiffuseTexture.GatherAlpha(DefaultSampler, In.vTexUV);
    //Out.vColor.a = Ahlpa.r;

    return Out;
}

PS_OUT PS_MAIN_CURCLE_BAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    if(g_Ratio <= 0.f)
    {
        Out.vColor.rgb = float3(0.85f, 0.f, 0.f);
        return Out;
    }
    
    // 중점에서 UV 좌표로 가는 방향 벡터를 구한다.
    float2 vDir = In.vTexUV - float2(0.5f, 0.5f);
    vDir = normalize(vDir);
    
    // 방향 벡터 x가 -면 비교할 기준 벡터의 방향은 위
    // 방향 벡터 x가 +면 비교할 기준 벡터의 방향은 아래
    float2 vUpDir = float2(0.0f, sign(vDir.x));
    vUpDir = normalize(vUpDir);
    
    float fDot = dot(vUpDir, vDir);
    float fDotRatio = g_Ratio;
    
    if (vDir.x < 0.f)
    {
        fDotRatio -= 0.5f;
    }
    
        
    fDotRatio = fDotRatio * 4.f - 1.f;

    if (fDotRatio < fDot)
        discard;
    

    return Out;
}

PS_OUT PS_MAIN_VIRTICAL_BAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    //Out.vColor.a *= g_AhlpaScale;
	
    float fCull = sign(g_Ratio - In.vTexUV.y);
    
    if (fCull < 0.f)
        discard;
   
    
    //float4 Ahlpa = g_DiffuseTexture.GatherAlpha(DefaultSampler, In.vTexUV);
    //Out.vColor.a = Ahlpa.r;

    return Out;
}

PS_OUT PS_MAIN_HORIZONTAL_BAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    //Out.vColor.a *= g_AhlpaScale;
	
    float fCull = sign(g_Ratio - In.vTexUV.x);
    
    if (fCull < 0.f)
        discard;
    
    //float4 Ahlpa = g_DiffuseTexture.GatherAlpha(DefaultSampler, In.vTexUV);
    //Out.vColor.a = Ahlpa.r;

    return Out;
}


PS_OUT PS_MAIN_CURCLE_BLEND_BAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    float fRatio = max(0.f, g_Ratio);
    fRatio = fRatio;
    
    // 중점에서 UV 좌표로 가는 방향 벡터를 구한다.
    float2 vDir = In.vTexUV - float2(0.5f, 0.5f);
    vDir = normalize(vDir);
    
    // 방향 벡터 x가 -면 비교할 기준 벡터의 방향은 위
    // 방향 벡터 x가 +면 비교할 기준 벡터의 방향은 아래
    float2 vUpDir = float2(0.0f, sign(vDir.x));
    vUpDir = normalize(vUpDir);
    
    float fDot = dot(vUpDir, vDir);
    float fDotRatio = g_Ratio;
    
    if (vDir.x < 0.f)
    {
        fDotRatio -= 0.5f;
    }
    
        
    fDotRatio = fDotRatio * 4.f - 1.f;

    if (fDotRatio >= fDot)
    {
        Out.vColor.rgb *= 0.3f;
    }
    

        return Out;
    }


technique11 DefaultTechnique
{
	pass Default //0
	{
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    pass UI_MonsterHP //1
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MONSTER_HPBAR();
    }

    pass UI_PlayerHP //2
    {
        SetBlendState(BS_None, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_PLAYER_HPBAR();
    }

    pass Backgorund //3
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass UI_ComboBar //4
    {
        SetBlendState(BS_None, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_COMBO_BAR();
    }

    pass UI_CurcleBar //5
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CURCLE_BAR();
    }

    pass UI_VirticalBar //6
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_VIRTICAL_BAR();
    }

    pass UI_HorizontalBar //7
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HORIZONTAL_BAR();
    }

    pass UI_CurcleBlendBar //8
    {
        SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None_ZTest_And_Write, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CURCLE_BLEND_BAR();
    }

	/*pass Default
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass Default
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}*/
}