#include "stdafx.h"
#include "UIManager.h"
#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "ItemPopup_Queue.h"
#include "UI_Cursor.h"


IMPLEMENT_SINGLETON(CUIManager)

void CUIManager::Set_OpenedMenu(_bool bOpenedMenu)
{
	m_bOpenedMenu = bOpenedMenu;
}

POINT CUIManager::Get_MousePoint()
{
	POINT	tMousePt;

	GetCursorPos(&tMousePt);
	ClientToScreen(g_hWnd, &tMousePt);

	return tMousePt;
}

void CUIManager::EnableCursor()
{
	m_pCursor.lock()->Set_Enable(true);
}

void CUIManager::DisableCursor()
{
	m_pCursor.lock()->Set_Enable(false);
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

void CUIManager::Free()
{
	
}
