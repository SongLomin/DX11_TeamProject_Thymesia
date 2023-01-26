#include "stdafx.h"
#include "UI_FadeMask.h"
#include "EasingComponent_Alpha.h"
#include "EasingComponent_Transform.h"
#include "EasingComponent_Float.h"


GAMECLASS_C(CUI_FadeMask)
CLONE_C(CUI_FadeMask, CGameObject)

HRESULT CUI_FadeMask::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CUI_FadeMask::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

    Set_Texture("NoneBlack");

    m_pEasingAlpha = Add_Component<CEasingComponent_Alpha>();
    m_pEasingTimer = Add_Component<CEasingComponent_Float>();

    Set_Enable(false);
    return S_OK;
}

HRESULT CUI_FadeMask::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_FadeMask::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (m_pEasingAlpha.lock()->Is_Lerping())
    {
        Set_AlphaColor(m_pEasingAlpha.lock()->Get_Lerp());
    }
}

void CUI_FadeMask::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

void CUI_FadeMask::Set_Fade_Delay(_float fStart, _float fTarget, _float fTime, _float fDelay, EASING_TYPE eType)
{
    Set_Enable(true);

    m_pEasingAlpha.lock()->Set_Lerp_once(fStart, fTarget, fTime, eType, true);

    m_pEasingAlpha.lock()->Callback_LerpEnd += bind(&CUI_FadeMask::Call_OnLerpEnd_Delay, this);

    m_fDelay = fDelay;
}


void CUI_FadeMask::Set_Fade(_float fStart, _float fTarget, _float fTime, EASING_TYPE eType)
{
    Set_Enable(true);

    m_pEasingAlpha.lock()->Set_Lerp_once(fStart, fTarget, fTime, eType, true);

    m_pEasingAlpha.lock()->Callback_LerpEnd += bind(&CUI_FadeMask::Call_OnLerpEnd, this);
    m_pEasingAlpha.lock()->Callback_LerpEnd += bind(&CUI_FadeMask::Call_OnLerpEndEnable, this);
    m_pEasingAlpha.lock()->Callback_LerpEnd += bind(&CUI_FadeMask::Call_OnLerpEndDisable, this);
}

void CUI_FadeMask::Call_OnLerpEnd()
{
    Callback_OnLerpEnd();
    Callback_OnLerpEnd.Clear();

    Set_Enable(false);

}

void CUI_FadeMask::Call_OnLerpEndEnable()
{
    if (Callback_OnLerpEndSetEnable.empty())
        return;

    Callback_OnLerpEndSetEnable(nullptr);

    Callback_OnLerpEndSetEnable.Clear();

}

void CUI_FadeMask::Call_OnLerpEndDisable()
{
    if (Callback_OnLerpEndSetDisable.empty())
        return;

    Callback_OnLerpEndSetDisable();
    Callback_OnLerpEndSetDisable.Clear();
}

void CUI_FadeMask::Call_OnLerpEnd_Delay()
{
    m_pEasingTimer.lock()->Set_Lerp(0.f, m_fDelay, m_fDelay, EASING_TYPE::LINEAR, CEasingComponent::ONCE);
    m_pEasingTimer.lock()->Callback_LerpEnd += bind(&CUI_FadeMask::Call_EndTimer, this);
}

void CUI_FadeMask::Call_EndTimer()
{
    Set_Enable(false);
}

void CUI_FadeMask::Free()
{

}
