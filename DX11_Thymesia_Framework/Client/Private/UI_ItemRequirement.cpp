#include "stdafx.h"
#include "UI_ItemRequirement.h"
#include "UI_Utils.h"
#include "EasingComponent_Alpha.h"
#include "EasingComponent_Float.h"


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

    Set_UIPosition(360.f, 287.f, 881.f, 68.f, CUI::ALIGN_LEFTTOP);

    Set_AlphaColor(0.0f);
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
        Set_Texture("ItemRequirement_GardenKey");
        break;
    case Client::ITEM_NAME::VARG_KEY:
        Set_Texture("ItemRequirement_VargKey");
        break;
    default:
        return;
        break;
    }
    Start_UI();
}

void CUI_ItemRequirement::Call_RequirementDir()
{
    Set_Texture("Requirement_Direction");
    
    Start_UI();
}

void CUI_ItemRequirement::Call_UseItem(ITEM_NAME eITemName)
{
    switch (eITemName)
    {
    case Client::ITEM_NAME::GARDEN_KEY:
        Set_Texture("Use_GardenKey");
        break;
    case Client::ITEM_NAME::VARG_KEY:
        Set_Texture("Use_VargKey");
        break;
    default:
        return;
        break;
    }
    Start_UI();
}

void CUI_ItemRequirement::Call_EndLerp()
{
    Set_Texture("None");
    Set_Enable(false);
    m_bIsAnimation = false;
}


void CUI_ItemRequirement::Start_UI()
{
    //애니메이션이 돌고있을때는 재호출 X
    if (m_bIsAnimation)
        return;

    Set_Enable(true);
    m_bIsAnimation = true;
    m_pEasingAlpha.lock()->Set_Lerp(0.f, 0.9f, 1.f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);
    m_pEasingAlpha.lock()->Callback_LerpEnd += bind(&CUI_ItemRequirement::Call_EndFadeIn, this);
}

void CUI_ItemRequirement::Call_EndFadeIn()
{
    m_pEasingFloat.lock()->Set_Lerp(0.f, 0.9f, 2.f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE);
    m_pEasingFloat.lock()->Callback_LerpEnd += bind(&CUI_ItemRequirement::Call_EndTimeOut, this);
}

void CUI_ItemRequirement::Call_EndTimeOut()
{
    m_pEasingAlpha.lock()->Set_Lerp(0.9f, 0.f, 1.f, EASING_TYPE::QUAD_OUT, CEasingComponent::ONCE, false);
    m_pEasingAlpha.lock()->Callback_LerpEnd += bind(&CUI_ItemRequirement::Call_EndLerp, this);

}


void CUI_ItemRequirement::SetUp_Component()
{
    m_pEasingAlpha = Add_Component<CEasingComponent_Alpha>();
    m_pEasingFloat = Add_Component<CEasingComponent_Float>();

}
