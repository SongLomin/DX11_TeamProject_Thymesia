#include "stdafx.h"
#include "UIManager.h"
#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "ItemPopup_Queue.h"
#include "UI_Cursor.h"
#include "GameManager.h"
#include "UI_RadialBlurMask.h"
#include "UI_EvolveMenu.h"
#include "UI_FadeMask.h"

IMPLEMENT_SINGLETON(CUIManager)

void CUIManager::Set_OpenedMenu(_bool bOpenedMenu)
{
	m_bOpenedMenu = bOpenedMenu;
}

void CUIManager::OnEnterStage()
{

}

void CUIManager::Initalize()
{
	
}

void CUIManager::Tick(_float fTimeDelta)
{

}

void CUIManager::LateTick(_float fTimeDelta)
{

}

POINT CUIManager::Get_MousePoint()
{
	m_MousePt = { 0, 0 };

	GetCursorPos(&m_MousePt);
	ScreenToClient(g_hWnd, &m_MousePt);
	return m_MousePt;
}

void CUIManager::EnableCursor()
{
	m_pCursor.lock()->Set_Enable(true);
}

void CUIManager::DisableCursor()
{
	m_pCursor.lock()->Set_Enable(false);
}

void CUIManager::Add_SoundType(UI_SOUND_TYPE eType, _float fAmount)
{
	m_UISoundTypeMap.emplace(eType, fAmount);
}

_float CUIManager::Get_SoundType(UI_SOUND_TYPE eType)
{
	UISOUNDTYPE::iterator pair = m_UISoundTypeMap.find(eType);

	if (pair == m_UISoundTypeMap.end())
	{
		return 1.f;
	}
	return pair->second;
}

void CUIManager::Set_CloseCurtain(_float fTime)
{
	weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Get_GameObjects<CUI_FadeMask>(LEVEL_STATIC).front();
	weak_ptr<CUI_RadialBlurMask> pRadialBlurMask = GAMEINSTANCE->Get_GameObjects<CUI_RadialBlurMask>(LEVEL_STATIC).front();


	pRadialBlurMask.lock()->Set_Radial(0.0f, 0.3f, 0.5f);
	pFadeMask.lock()->Set_Fade(0.f, 1.f, 0.5f, EASING_TYPE::LINEAR);

	pFadeMask.lock()->Callback_OnLerpEnd += bind(&CUIManager::OpenCurtain, this);

}

void CUIManager::OpenCurtain()
{
	weak_ptr<CUI_RadialBlurMask> pRadialBlurMask = GAMEINSTANCE->Get_GameObjects<CUI_RadialBlurMask>(LEVEL_STATIC).front();
	weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Get_GameObjects<CUI_FadeMask>(LEVEL_STATIC).front();

	pRadialBlurMask.lock()->Set_Radial(0.3f, 0.0f, 0.5f);
	pFadeMask.lock()->Set_Fade(1.f, 0.f, 0.5f, EASING_TYPE::LINEAR);

}


void CUIManager::OnEnterEvolveMenu()
{
	weak_ptr<CUI_RadialBlurMask> pRadialBlurMask = GAMEINSTANCE->Get_GameObjects<CUI_RadialBlurMask>(LEVEL_STATIC).front();
	weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Get_GameObjects<CUI_FadeMask>(LEVEL_STATIC).front();

	pRadialBlurMask.lock()->Set_Radial(0.0f, 0.3f, 0.3f);
	pRadialBlurMask.lock()->Callback_OnEndLerp += bind(&CUIManager::Open_EvolveMenu, this);

	pFadeMask.lock()->Set_Fade(0.f, 1.f, 0.3f, EASING_TYPE::LINEAR);
	GAMEINSTANCE->PlaySound2D("EvolveMenu_Enable.wav", m_UISoundTypeMap[UI_SOUND_TYPE::SOUND_CHOOSE_SELECT]);
}

void CUIManager::OnExitEvolveMenu()
{
	weak_ptr<CUI_EvolveMenu> pEvolveMenu = GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu>(LEVEL_STATIC).front();
	pEvolveMenu.lock()->Set_Enable(false);

	weak_ptr<CUI_RadialBlurMask> pRadialBlurMask = GAMEINSTANCE->Get_GameObjects<CUI_RadialBlurMask>(LEVEL_STATIC).front();
	weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Get_GameObjects<CUI_FadeMask>(LEVEL_STATIC).front();

	pRadialBlurMask.lock()->Set_Radial(0.3f, 0.0f, 0.7f, EASING_TYPE::QUART_IN);
	pFadeMask.lock()->Set_Fade(1.f, 0.f, 0.7f, EASING_TYPE::QUART_IN);

	pFadeMask.lock()->Callback_OnLerpEnd += bind(&CUI_EvolveMenu::Call_FadeEndEnableEvolveMenu, pEvolveMenu.lock());
	GAMEINSTANCE->PlaySound2D("EvolveMenu_Disable.wav", m_UISoundTypeMap[UI_SOUND_TYPE::SOUND_CHOOSE_SELECT]);

}

void CUIManager::CreateItemPopupQueue()
{
	if (m_pItemPopupQueue)
		return;

	m_pItemPopupQueue = CItemPopup_Queue::Create();
}

void CUIManager::Add_ItemPopup(ITEM_NAME eItemName)
{
	m_pItemPopupQueue->AddPopup(eItemName);

}

void CUIManager::Open_EvolveMenu()
{
	weak_ptr<CUI_EvolveMenu> pEvolveMenu = GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu>(LEVEL_STATIC).front();
	pEvolveMenu.lock()->Set_Enable(true);

	weak_ptr<CUI_RadialBlurMask> pRadialBlurMask = GAMEINSTANCE->Get_GameObjects<CUI_RadialBlurMask>(LEVEL_STATIC).front();
	weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Get_GameObjects<CUI_FadeMask>(LEVEL_STATIC).front();

	pRadialBlurMask.lock()->Set_Radial(0.3f, 0.f, 0.3f);
	pFadeMask.lock()->Set_Fade(1.f, 0.f, 0.3f, EASING_TYPE::LINEAR);

}


void CUIManager::Free()
{
	
}
