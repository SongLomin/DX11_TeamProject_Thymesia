#include "stdafx.h"
#include "UI_EvolveMenu_Page.h"
#include "FadeMask.h"
#include "UI_EvolveMenu.h"
#include "UIManager.h"


GAMECLASS_C(CUI_EvolveMenu_Page)
CLONE_C(CUI_EvolveMenu_Page, CGameObject)

HRESULT CUI_EvolveMenu_Page::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

    return S_OK;
}

void CUI_EvolveMenu_Page::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (KEY_INPUT(KEY::T, KEY_STATE::TAP))
    {
        Update_KeyInput(fTimeDelta);
    }
}

void CUI_EvolveMenu_Page::Update_KeyInput(_float fTimeDelta)
{
    GET_SINGLE(CUIManager)->Set_OpenedMenu(false);
    FaderDesc tFaderDesc;
    tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
    tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
    tFaderDesc.fFadeMaxTime = 0.2f;
    tFaderDesc.fDelayTime = 0.f;
    tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

    m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
    m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_EvolveMenu_Page::Call_ExitTap, this);
    GET_SINGLE(CUIManager)->Set_OpenedMenu(false);
}

void CUI_EvolveMenu_Page::Call_ExitTap()
{
    Set_Enable(false);
    m_pFadeMask.lock()->Set_Enable(false);
    GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu>(LEVEL_STATIC).front().lock()->Set_Enable(true);
    GAMEINSTANCE->PlaySound2D("Fantasy_Game_UI_Ice_Select.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT));

}

void CUI_EvolveMenu_Page::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    GET_SINGLE(CUIManager)->Set_OpenedMenu(true);
}

void CUI_EvolveMenu_Page::OnDisable()
{
    __super::OnDisable();

    GET_SINGLE(CUIManager)->Set_OpenedMenu(false);
}
