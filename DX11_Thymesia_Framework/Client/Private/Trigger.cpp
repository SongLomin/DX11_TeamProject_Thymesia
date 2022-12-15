#include "stdafx.h"
#include "Trigger.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Collider.h"

GAMECLASS_C(CTrigger);

HRESULT CTrigger::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CTrigger::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pTriggerColliderCom = Add_Component<CCollider>();

    return S_OK;
}

HRESULT CTrigger::Start()
{
    return S_OK;
}

void CTrigger::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    m_pTriggerColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CTrigger::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CTrigger::Render(ID3D11DeviceContext* pDeviceContext)
{
    SetUp_ShaderResource();

    __super::Render(pDeviceContext);
    return S_OK;
}

void CTrigger::SetUp_ShaderResource()
{
    
}

void CTrigger::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CTrigger::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CTrigger::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CTrigger::OnEventMessage(_uint iArg)
{
    __super::OnEventMessage(iArg);
}

void CTrigger::Free()
{
}
