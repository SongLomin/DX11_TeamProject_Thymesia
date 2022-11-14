#include "stdafx.h"
#include "Actor.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "RigidBody.h"
#include "GameManager.h"

GAMECLASS_C(CActor)
CLONE_C(CActor, CGameObject)

HRESULT CActor::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CActor::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pModelCom = Add_Component<CModel>();
    m_pShaderCom = Add_Component<CShader>();
    m_pRendererCom = Add_Component<CRenderer>();
    m_pRigidBodyCom = Add_Component<CRigidBody>();

    return S_OK;
}

void CActor::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CActor::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Cast<CGameObject>(m_this));
}

HRESULT CActor::Render()
{
    SetUp_ShaderResource();
    
    //m_pShaderCom.lock()->Begin(m_iPassIndex);

    __super::Render();

    return S_OK;
}

_vector CActor::Get_WorldPosition()
{
    return m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
}



void CActor::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
    GET_SINGLE(CGameManager)->Active_KeyEvent(m_pModelCom, m_pTransformCom, In_iKeyIndex);

}

void CActor::SetUp_ShaderResource()
{
    //CallBack_Bind_SRV(m_pShaderCom, "");

    m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
    m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));
}

void CActor::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    for (auto& elem : m_EffectIndexList)
    {
        elem.second = GET_SINGLE(CGameManager)->Use_EffectGroup(elem.first, m_pTransformCom);
    }

}

void CActor::OnDisable()
{
    __super::OnDisable();

    for (auto& elem : m_EffectIndexList)
    {
        GET_SINGLE(CGameManager)->UnUse_EffectGroup(elem.first, elem.second);
    }
}

void CActor::Free()
{
}
