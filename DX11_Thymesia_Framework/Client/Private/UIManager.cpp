#include "stdafx.h"
#include "UIManager.h"
#include "Client_Defines.h"
#include "Engine_Defines.h"

IMPLEMENT_SINGLETON(CUIManager)

void CUIManager::Set_OpenedMenu(_bool bOpenedMenu)
{
	m_bOpenedMenu = bOpenedMenu;
}

void CUIManager::Free()
{
	
}
