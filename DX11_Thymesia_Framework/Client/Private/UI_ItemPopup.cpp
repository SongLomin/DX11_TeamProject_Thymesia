#include "stdafx.h"
#include "UI_ItemPopup.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "Preset_UIDesc.h"
#include "Item.h"
#include "EasingComponent_Float.h"
#include "Preset_UIDesc.h"
#include "EasingComponent_Alpha.h"
#include "RequirementChecker.h"

GAMECLASS_C(CUI_ItemPopup)
CLONE_C(CUI_ItemPopup, CGameObject)

HRESULT CUI_ItemPopup::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CUI_ItemPopup::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    RENDERGROUP::RENDER_BEFOREUI;

   

    Set_Texture("None");

    SetUp_Component();

    Set_Enable(false);

    
    return S_OK;
}

HRESULT CUI_ItemPopup::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_ItemPopup::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);


    if (m_pEasingFloat.lock()->Is_Lerping())
    {
        Set_X(m_pEasingFloat.lock()->Get_Lerp());
    }
}

void CUI_ItemPopup::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);

}

void CUI_ItemPopup::Reset()
{
    m_pEasingFloat.lock()->Stop();
    m_pEasingAlpha.lock()->Stop();
    m_pEasingAlarm.lock()->Stop();
}

void CUI_ItemPopup::Ready_Popup(ITEM_NAME eItemName, _float fLifeTime)
{
    m_fLifeTime = fLifeTime;

    CPreset_UIDesc::Set_CUI_ItemPopup_Ready_Popup(Weak_StaticCast<CUI>(m_this), eItemName);
    Set_Enable(true);
    Set_UIPosition(8000.f, 209.f, 552.f, 98.f, CUI::ALIGN_LEFTTOP);
}

void CUI_ItemPopup::Start_Popup()
{
    Set_Enable(true);

    Set_UIPosition(1634.f, 209.f, 552.f, 98.f, CUI::ALIGN_LEFTTOP);

    m_pEasingFloat.lock()->Set_Lerp(1634.f, 1561.f, 0.3f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE);

    m_pEasingAlpha.lock()->Set_Lerp(0.f, 1.f, 0.3f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);

    m_pEasingFloat.lock()->Callback_LerpEnd +=
        bind(&CUI_ItemPopup::Call_EndFadeIn, this);
}

void CUI_ItemPopup::SetUp_Component()
{
    m_pEasingFloat = Add_Component<CEasingComponent_Float>();
    m_pEasingAlpha = Add_Component<CEasingComponent_Alpha>();

    m_pEasingAlarm = Add_Component<CEasingComponent_Float>();
}

void CUI_ItemPopup::Call_EndFadeIn()
{
    m_pEasingAlarm.lock()->Set_Lerp(0.f, 1.f, m_fLifeTime, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE);
    
    m_pEasingAlarm.lock()->Callback_LerpEnd +=
        bind(&CUI_ItemPopup::Call_StartFadeOut, this);
}

void CUI_ItemPopup::Call_StartFadeOut()
{
    m_pEasingFloat.lock()->Set_Lerp(1561.f, 1634.f, 0.3f, EASING_TYPE::QUAD_OUT, CEasingComponent::ONCE);
    m_pEasingAlpha.lock()->Set_Lerp(1.f, 0.f, 0.3f, EASING_TYPE::QUAD_OUT, CEasingComponent::ONCE, false);

    m_pEasingFloat.lock()->Callback_LerpEnd +=
        bind(&CUI_ItemPopup::Call_EndFadeOut, this);

}

void CUI_ItemPopup::Call_EndFadeOut()
{
    Set_UIPosition(1634.f, 209.f, 552.f, 98.f, CUI::ALIGN_LEFTTOP);
    Callback_EndFadeOut(Weak_StaticCast<CUI_ItemPopup>(m_this));

    Callback_EndFadeOut.Clear();
    Set_Enable(false);

}
