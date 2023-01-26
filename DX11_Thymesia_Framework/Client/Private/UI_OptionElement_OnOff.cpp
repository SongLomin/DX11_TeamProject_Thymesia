#include "stdafx.h"
#include "UI_OptionElement_OnOff.h"
#include "CustomUI.h"
#include "UI_Utils.h"

GAMECLASS_C(CUI_OptionElement_OnOff)
CLONE_C(CUI_OptionElement_OnOff, CGameObject)


HRESULT CUI_OptionElement_OnOff::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    SetUp_ArrowPos();

    m_pOptionState = ADD_STATIC_CUSTOMUI;

    m_pOptionState.lock()->Set_UIPosition
    (
        463.f,
        400.f,
        61.f,
        35.f,
        ALIGN_LEFTTOP
    );
    m_pOptionState.lock()->Set_Depth(0.6f);
    m_pOptionState.lock()->Set_Texture("EvolveMenu_Option_Off");

    Add_Child(m_pOptionState);

    m_pOptionState.lock()->Set_Enable(false);

    return S_OK;
}

HRESULT CUI_OptionElement_OnOff::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_OptionElement_OnOff::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

}

void CUI_OptionElement_OnOff::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);
}

void CUI_OptionElement_OnOff::Call_OnClickLeftArrow()
{
    ClickArrow();
}

void CUI_OptionElement_OnOff::Call_OnClickRightArrow()
{
    ClickArrow();
}

void CUI_OptionElement_OnOff::SetUp_ArrowPos()
{
    m_pLeftArrow.lock()->Set_UIPosition
    (
        383.f,
        392.f,
        20.f,
        50.f,
        ALIGN_LEFTTOP
    );

    m_pRightArrow.lock()->Set_UIPosition
    (
        580.f,
        392.f,
        20.f,
        50.f,
        ALIGN_LEFTTOP
    );
}

void CUI_OptionElement_OnOff::ClickArrow()
{
    if (m_iAmount == 0)
    {
        m_iAmount = 1;
        m_pOptionState.lock()->Set_Texture("EvolveMenu_Option_On");
    }
    else if (m_iAmount == 1)
    {
        m_iAmount = 0;
        m_pOptionState.lock()->Set_Texture("EvolveMenu_Option_Off");
    }
    Callback_OnChangeOption(m_iAmount);
}

void CUI_OptionElement_OnOff::Free()
{

}
