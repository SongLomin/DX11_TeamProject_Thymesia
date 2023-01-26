#include "stdafx.h"
#include "UI_Effect_Twinkl.h"
#include "UI_Utils.h"
#include "UIManager.h"

#include "EasingComponent_Alpha.h"
#include "EasingComponent_Transform.h"


GAMECLASS_C(CUI_Effect_Twinkl)
CLONE_C(CUI_Effect_Twinkl, CGameObject)

HRESULT CUI_Effect_Twinkl::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CUI_Effect_Twinkl::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Set_Texture("HighLight");

    m_iMaskIndex = 101;
    m_iPassIndex = 1;
    m_iDeffuseIndex = 0;

    Set_Depth(0.5f);

    return S_OK;
}

HRESULT CUI_Effect_Twinkl::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_Effect_Twinkl::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

    if (m_pEasingSizing.lock()->Is_Lerping())
    {
        _float2 fSize = m_pEasingSizing.lock()->Get_Lerp();

        Set_Size(fSize.x, fSize.y);

    }
}

void CUI_Effect_Twinkl::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);
}

void CUI_Effect_Twinkl::Play(_float2 fPt, _float fSizeMag)
{
    Set_Enable(true);

    Set_UIPosition(fPt.x, fPt.y);

    m_fPlayTime = 0.5f;

    m_pEasingAlpha.lock()->Set_Lerp_once(1.0f, 0.0f, m_fPlayTime, EASING_TYPE::SINE_IN, false);
    m_pEasingSizing.lock()->Set_Lerp
    (
        _float2(128.f * fSizeMag, 128.f * fSizeMag),
        _float2(196.f * fSizeMag, 196.f * fSizeMag),
        m_fPlayTime, EASING_TYPE::SINE_IN, CEasingComponent::ONCE, true
    );

    //    m_pEasingAlpha.lock()->Callback_LerpEnd += bind(&CUI_Effect_MagicCircle::Init_UI, this);
}

void CUI_Effect_Twinkl::Init_UI()
{
    __super::Init_UI();

    Set_Size(128.f, 128.f);
}

void CUI_Effect_Twinkl::Free()
{
}

