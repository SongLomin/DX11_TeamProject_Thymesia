#include "stdafx.h"
#include "Dynamic_Prop.h"
#include "Client_Components.h"

GAMECLASS_C(CDynamic_Prop);
CLONE_C(CDynamic_Prop, CGameObject);

HRESULT CDynamic_Prop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDynamic_Prop::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom = Add_Component<CCollider>();
    //m_pColliderCom.lock()->Init_Collider();

    return S_OK;
}

HRESULT CDynamic_Prop::Start()
{
    return S_OK;
}

void CDynamic_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CDynamic_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CDynamic_Prop::Render()
{
    __super::Render();

    return S_OK;
}

void CDynamic_Prop::Free()
{
}
