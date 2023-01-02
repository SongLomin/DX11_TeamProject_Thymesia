#include "Client_Shader_Defines.hpp"

#define PATCH_SIZE 3

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4 g_vShaderFlag;

texture2D g_Texture_Sorc_Diff, g_Texture_Sorc_Norm;
float g_fSorc_Density;
texture2D g_Texture_AddNo1_Diff, g_Texture_AddNo1_Norm;
float g_fAddNo1_Density;
texture2D g_Texture_AddNo2_Diff, g_Texture_AddNo2_Norm;
float g_fAddNo2_Density;
texture2D g_Texture_AddNo3_Diff, g_Texture_AddNo3_Norm;
float g_fAddNo3_Density;
texture2D g_FilterTexture;

texture2D g_NoiseTexture1;
texture2D g_NoiseTexture2;
texture2D g_DisplacementTexture;

float g_fFar = 300.f;

float2 g_vUVNoise;
/* ---------------------------------------------------------- */

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal   : NORMAL;
    float2 vTexUV    : TEXCOORD0;
    float3 vTangent  : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal   : NORMAL;
    float2 vTexUV    : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos  : TEXCOORD2;
    float3 vTangent  : TANGENT;
    float3 vBinormal : BINORMAL;
};

/* ---------------------------------------------------------- */

VS_OUT VS_MAIN_DEFAULT(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;
    
    //vector vDisplacement = g_DisplacementTexture.SampleLevel(DefaultSampler, In.vTexUV*10.f + g_vUVNoise*0.1f, 0);

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
   // In.vPosition += vDisplacement*0.8f;
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
    Out.vBinormal = normalize(cross(float3(Out.vNormal.xyz), Out.vTangent));

    return Out;
}
struct VS_OUT_HULL
{
    float4 vPosition : SV_POSITION;
    float4 vNormal   : NORMAL;
    float2 vTexUV    : TEXCOORD0;    
    float3 vTangent  : TANGENT;
};

/* ---------------------------------------------------------- */

VS_OUT_HULL VS_MAIN_HULL(VS_IN In)
{
    VS_OUT_HULL Out = (VS_OUT_HULL) 0;

    Out.vPosition = vector(In.vPosition, 1.f); //mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
    //Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    Out.vNormal = vector(In.vNormal, 0.f);
    Out.vTangent = In.vTangent;

    return Out;
}


/*.........hull/domain shader.......................*/

struct PatchTess
{
    float edgeTess[3] : SV_TessFactor;
    float insideTess  : SV_InsideTessFactor;
};

struct HS_OUT
{
	float4 vPosition : SV_POSITION;
	float4 vNormal   : NORMAL;
	float3 vTangent  : TANGENT;
	float2 vTexUV    : TEXCOORD0;
};

// Constant HS
PatchTess ConstantHS(InputPatch<VS_OUT_HULL, PATCH_SIZE> input, int patchID : SV_PrimitiveID)
{
    PatchTess output = (PatchTess) 0.f;
       
	output.edgeTess[0] = 15;
	output.edgeTess[1] = 15;
	output.edgeTess[2] = 15;
    output.insideTess = 1;
    
    return output;
}

// Control Point HS
[domain("tri")] // 패치의 종류 (tri, quad, isoline)
[partitioning("integer")] // subdivision mode (integer 소수점 무시, fractional_even, fractional_odd)
[outputtopology("triangle_cw")] // (triangle_cw, triangle_ccw, line)
[outputcontrolpoints(PATCH_SIZE)] // 하나의 입력 패치에 대해, HS가 출력할 제어점 개수
[patchconstantfunc("ConstantHS")] // ConstantHS 함수 이름
HS_OUT HS_Main(InputPatch<VS_OUT_HULL, PATCH_SIZE> input, int vertexIdx : SV_OutputControlPointID, int patchID : SV_PrimitiveID)
{
    HS_OUT output = (HS_OUT) 0.f;

    output.vPosition = input[vertexIdx].vPosition;
    output.vTexUV = input[vertexIdx].vTexUV;
    output.vNormal = input[vertexIdx].vNormal;
    output.vTangent = input[vertexIdx].vTangent;

    return output;
}

// --------------
// Domain Shader
// --------------

struct DS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal   : NORMAL;
    float2 vTexUV    : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos  : TEXCOORD2;
    float3 vTangent  : TANGENT;
    float3 vBinormal : BINORMAL;
};

[domain("tri")]
DS_OUT DS_Main(const OutputPatch<HS_OUT, PATCH_SIZE> input, float3 location : SV_DomainLocation, PatchTess patch)
{
    DS_OUT output = (DS_OUT) 0.f;

    float3 localPos = input[0].vPosition * location.r + input[1].vPosition * location.g + input[2].vPosition * location.b;
    float2 uv       = input[0].vTexUV    * location.r + input[1].vTexUV    * location.g + input[2].vTexUV    * location.b;
    float3 normal   = input[0].vNormal   * location.r + input[1].vNormal   * location.g + input[2].vNormal   * location.b;
    float3 tangent  = input[0].vTangent  * location.r + input[1].vTangent  * location.g + input[2].vTangent  * location.b;
    
    vector vDisplacement = ( /*1.f - */g_DisplacementTexture.SampleLevel(DefaultSampler, uv + g_vUVNoise * 0.005f, 0)) * 0.15f;
    
    matrix matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matrix matWVP = mul(matWV, g_ProjMatrix);
    
    output.vPosition = mul(float4(localPos + vDisplacement.xyz, 1.f), matWVP);
    output.vTexUV    = uv;
    output.vNormal   = normalize(mul(vector(normal, 0.f), g_WorldMatrix));
    output.vWorldPos = mul(vector(localPos, 1.f), g_WorldMatrix);
    output.vProjPos  = output.vPosition;
    output.vTangent  = normalize(mul(vector(tangent, 0.f), g_WorldMatrix)).xyz;
    output.vBinormal = normalize(cross(float3(output.vNormal.xyz), output.vTangent));
    return output;
}
/* ---------------------------------------------------------- */

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal   : NORMAL;
    float2 vTexUV    : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos  : TEXCOORD2;
    float3 vTangent  : TANGENT;
    float3 vBinormal : BINORMAL;
};

struct PS_OUT
{
    vector vDiffuse    : SV_TARGET0;
    vector vNormal     : SV_TARGET1;
    vector vDepth      : SV_TARGET2;
    vector vShaderFlag : SV_Target3;
    vector vORM        : SV_Target4;
};

/* ---------------------------------------------------------- */

PS_OUT PS_MAIN_DEFAULT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	Out.vDiffuse = g_Texture_Sorc_Diff.Sample(DefaultSampler, In.vTexUV * g_fSorc_Density);

    vector vFilterDiffuse = g_FilterTexture.Sample(DefaultSampler, In.vTexUV);

    if (0.1f < vFilterDiffuse.r)
    {
        vector AddTex = g_Texture_AddNo1_Diff.Sample(DefaultSampler, In.vTexUV * g_fAddNo1_Density);
        vector vFilter = vector(vFilterDiffuse.r, vFilterDiffuse.r, vFilterDiffuse.r, 1.f);

        Out.vDiffuse = AddTex * vFilter + Out.vDiffuse * (1.f - vFilter);
    }

    if (0.1f < vFilterDiffuse.g)
    {
        vector AddTex = g_Texture_AddNo2_Diff.Sample(DefaultSampler, In.vTexUV * g_fAddNo2_Density);
        vector vFilter = vector(vFilterDiffuse.g, vFilterDiffuse.g, vFilterDiffuse.g, 1.f);

        Out.vDiffuse = AddTex * vFilter + Out.vDiffuse * (1.f - vFilter);
    }

    if (0.1f < vFilterDiffuse.b)
    {
        vector AddTex = g_Texture_AddNo3_Diff.Sample(DefaultSampler, In.vTexUV * g_fAddNo3_Density);
        vector vFilter = vector(vFilterDiffuse.b, vFilterDiffuse.b, vFilterDiffuse.b, 1.f);

        Out.vDiffuse = AddTex * vFilter + Out.vDiffuse * (1.f - vFilter);
    }

    Out.vDiffuse.a = 1.f;
    Out.vNormal = In.vNormal;
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;
    Out.vORM = 0;

    return Out;
}

PS_OUT PS_MAIN_NORM(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_Texture_Sorc_Diff.Sample(DefaultSampler, In.vTexUV * g_fSorc_Density);

    vector vFilterDiffuse = g_FilterTexture.Sample(DefaultSampler, In.vTexUV);
    float3 vPixelNorm = g_Texture_Sorc_Norm.Sample(DefaultSampler, In.vTexUV * g_fSorc_Density).xyz;

    if (0.1f < vFilterDiffuse.r)
    {
        vector AddTex_Diff = g_Texture_AddNo1_Diff.Sample(DefaultSampler, In.vTexUV * g_fAddNo1_Density);
        vector AddTex_Norm = g_Texture_AddNo1_Norm.Sample(DefaultSampler, In.vTexUV * g_fAddNo1_Density);
        vector vFilter = vector(vFilterDiffuse.r, vFilterDiffuse.r, vFilterDiffuse.r, 1.f);

        Out.vDiffuse = AddTex_Diff * vFilter + Out.vDiffuse * (1.f - vFilter);
        vPixelNorm = AddTex_Norm.xyz;
    }

    if (0.1f < vFilterDiffuse.g)
    {
        vector AddTex_Diff = g_Texture_AddNo2_Diff.Sample(DefaultSampler, In.vTexUV * g_fAddNo2_Density);
        vector AddTex_Norm = g_Texture_AddNo2_Norm.Sample(DefaultSampler, In.vTexUV * g_fAddNo2_Density);
        vector vFilter = vector(vFilterDiffuse.g, vFilterDiffuse.g, vFilterDiffuse.g, 1.f);

        Out.vDiffuse = AddTex_Diff * vFilter + Out.vDiffuse * (1.f - vFilter);
        vPixelNorm = AddTex_Norm.xyz;
    }

    if (0.1f < vFilterDiffuse.b)
    {
        vector AddTex_Diff = g_Texture_AddNo3_Diff.Sample(DefaultSampler, In.vTexUV * g_fAddNo3_Density);
        vector AddTex_Norm = g_Texture_AddNo3_Norm.Sample(DefaultSampler, In.vTexUV * g_fAddNo3_Density);
        vector vFilter = vector(vFilterDiffuse.b, vFilterDiffuse.b, vFilterDiffuse.b, 1.f);

        Out.vDiffuse = AddTex_Diff * vFilter + Out.vDiffuse * (1.f - vFilter);
        vPixelNorm = AddTex_Norm.xyz;
    }
    //물쉐이더 테스트 용 

    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, float3(In.vNormal.xyz));
    vPixelNorm = vPixelNorm * 2.f - 1.f;
    vPixelNorm = mul(vPixelNorm, WorldMatrix);
       
    Out.vDiffuse.a  = 1.f;
    Out.vNormal     = vector(vPixelNorm.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;
    Out.vORM        = 0;

    return Out;
}

PS_OUT PS_MAIN_WIREFRAM(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = vector(1.f, 1.f, 0.f, 1.f);
    Out.vNormal = In.vNormal;
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;
    Out.vORM = 0;
    return Out;
}

PS_OUT PS_MAIN_FILLTER(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	Out.vDiffuse = g_Texture_Sorc_Diff.Sample(DefaultSampler, In.vTexUV * g_fSorc_Density);

    vector vFilterDiffuse = g_FilterTexture.Sample(DefaultSampler, In.vTexUV);

    if (0.1f < vFilterDiffuse.r)
        Out.vDiffuse = vFilterDiffuse;

    if (0.1f < vFilterDiffuse.g)
        Out.vDiffuse = vFilterDiffuse;

    if (0.1f < vFilterDiffuse.b)
        Out.vDiffuse = vFilterDiffuse;

    Out.vDiffuse.a = 1.f;
    Out.vNormal = In.vNormal;
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;
    Out.vORM = 0;

    return Out;
}

PS_OUT PS_MAIN_WATER(DS_OUT In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_Texture_Sorc_Diff.Sample(DefaultSampler, In.vTexUV * g_fSorc_Density);

    vector vFilterDiffuse = g_FilterTexture.Sample(DefaultSampler, In.vTexUV);
 
      //물쉐이더 테스트 용
    float3 vPixelNorm = g_NoiseTexture1.Sample(DefaultSampler, In.vTexUV*10.f + g_vUVNoise * 0.005f) * 2.f - 1.f;
   // vPixelNorm += g_NoiseTexture2.Sample(DefaultSampler, In.vTexUV * 30.f + g_vUVNoise * 0.1f) * 2.f - 1.f;
      
    vPixelNorm = float3(vPixelNorm.rg, lerp(1, vPixelNorm.b, 1.f));
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, float3(In.vNormal.xyz));
    vPixelNorm = mul(vPixelNorm, WorldMatrix);
 
    Out.vDiffuse = 0.1f * Out.vDiffuse + 0.9f * vector(0.8f, 0.f, 0.01f, 1.f);
      
    Out.vDiffuse.a = 1.f;
    Out.vNormal = vector(vPixelNorm.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
    Out.vShaderFlag = g_vShaderFlag;
    Out.vORM = 0;
    return Out;
}

/* ---------------------------------------------------------- */


technique11 DefaultTechnique
{
    pass PASS_0_Ground_Default
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader   = compile vs_5_0 VS_MAIN_DEFAULT();
        HullShader     = NULL;
        DomainShader   = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_DEFAULT();
    }

    pass PASS_1_Ground_Norm
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader   = compile vs_5_0 VS_MAIN_DEFAULT();
        HullShader     = NULL;
        DomainShader   = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_NORM();
    }

    pass PASS_2_Ground_WireFrame
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Wireframe);

        VertexShader   = compile vs_5_0 VS_MAIN_DEFAULT();
        HullShader     = NULL;
        DomainShader   = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_WIREFRAM();
    }

    pass PASS_3_Ground_OnlyFilterTexture
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader   = compile vs_5_0 VS_MAIN_DEFAULT();
        HullShader     = NULL;
        DomainShader   = NULL;
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_FILLTER();
    }
    
    pass Water // 4
    {
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 2);
        SetRasterizerState(RS_Default);

        VertexShader   = compile vs_5_0 VS_MAIN_HULL();
        HullShader     = compile hs_5_0 HS_Main();
        DomainShader   = compile ds_5_0 DS_Main();
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_WATER();

    }
}