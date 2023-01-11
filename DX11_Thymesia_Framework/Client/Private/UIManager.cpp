#include "stdafx.h"
#include "UIManager.h"
#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "ItemPopup_Queue.h"
#include "UI_Cursor.h"
#include "GameManager.h"
#include "UI_RadialBlurMask.h"
#include "UI_EvolveMenu.h"

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

void CUIManager::OnEnterEvolveMenu()
{
	weak_ptr<CUI_RadialBlurMask> pRadialBlurMask = GAMEINSTANCE->Get_GameObjects<CUI_RadialBlurMask>(LEVEL_STATIC).front();


	if (!pRadialBlurMask.lock())
	{
		pRadialBlurMask = GAMEINSTANCE->Add_SingleGameObject<CUI_RadialBlurMask>(LEVEL_STATIC);
	}
	pRadialBlurMask.lock()->Set_Radial(0.0f, 0.3f, 0.5f);

	pRadialBlurMask.lock()->Callback_OnEndLerp += bind(&CUIManager::Open_EvolveMenu, this);
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
}

void CUIManager::Free()
{
	
}
