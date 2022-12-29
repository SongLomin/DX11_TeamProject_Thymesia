#include "stdafx.h"
#include "UI_ItemRequirement.h"
#include "UI_Utils.h"
#include "EasingComponent_Alpha.h"

GAMECLASS_C(CUI_ItemRequirement)
CLONE_C(CUI_ItemRequirement, CGameObject)

HRESULT CUI_ItemRequirement::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CUI_ItemRequirement::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Set_Texture("None");

    SetUp_Component();

    return S_OK;
}

HRESULT CUI_ItemRequirement::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_ItemRequirement::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

}

void CUI_ItemRequirement::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);

}

void CUI_ItemRequirement::Call_ItemRequireMent(ITEM_NAME eITemName)
{
    switch (eITemName)
    {

    case Client::ITEM_NAME::GARDEN_KEY:
        break;
    case Client::ITEM_NAME::VARG_KEY:
        break;
    default:
        return;
        break;
    }
    m_pEasingAlpha.lock()->Set_Lerp(0.f, 1.f, 1.f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);

}

void CUI_ItemRequirement::Call_EndLerp()
{
    Set_Texture("None");
    Set_Enable(false);
}

void CUI_ItemRequirement::SetUp_Component()
{
    m_pEasingAlpha = Add_Component<CEasingComponent_Alpha>();
}
