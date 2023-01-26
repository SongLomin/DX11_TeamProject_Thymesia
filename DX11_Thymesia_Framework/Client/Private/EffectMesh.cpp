#include "stdafx.h"
#include "EffectMesh.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "RigidBody.h"

GAMECLASS_C(CEffectMesh)
CLONE_C(CEffectMesh, CGameObject)

HRESULT CEffectMesh::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffectMesh::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pModelCom = Add_Component<CModel>();
    m_pShaderCom = Add_Component<CShader>();
    m_pRendererCom = Add_Component<CRenderer>();

    return S_OK;
}

void CEffectMesh::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CEffectMesh::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Cast<CGameObject>(m_this));
}

HRESULT CEffectMesh::Render(ID3D11DeviceContext* pDeviceContext)
{
    SetUp_ShaderResource();

    //m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);

    __super::Render(pDeviceContext);

    return S_OK;
}

void CEffectMesh::SetUp_ShaderResource()
{
    //CallBack_Bind_SRV(m_pShaderCom, "");

    m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
    m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

   /* const LIGHTDESC& LightDesc = GAMEINSTANCE->Get_LightDesc(0);

    m_pShaderCom.lock()->Set_RawValue("g_vCamPosition", &GAMEINSTANCE->Get_CamPosition(), sizeof(_float4));
    m_pShaderCom.lock()->Set_RawValue("g_vLightDir", (void*)&LightDesc.vDirection, sizeof(_float4));
    m_pShaderCom.lock()->Set_RawValue("g_vLightPos", (void*)&LightDesc.vPosition, sizeof(_float4));
    m_pShaderCom.lock()->Set_RawValue("g_fRange", (void*)&LightDesc.fRange, sizeof(_float));

    m_pShaderCom.lock()->Set_RawValue("g_vLightDiffuse", (void*)&LightDesc.vDiffuse, sizeof(_float4));
    m_pShaderCom.lock()->Set_RawValue("g_vLightAmbient", (void*)&LightDesc.vAmbient, sizeof(_float4));
    m_pShaderCom.lock()->Set_RawValue("g_vLightSpecular", (void*)&LightDesc.vSpecular, sizeof(_float4));*/
}

void CEffectMesh::Free()
{
}
