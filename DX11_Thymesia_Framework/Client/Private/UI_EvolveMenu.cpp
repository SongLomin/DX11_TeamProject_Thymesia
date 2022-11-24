#include "stdafx.h"
#include "UI_EvolveMenu.h"
#include "UI_Elements.h"



GAMECLASS_C(CUI_EvolveMenu)
CLONE_C(CUI_EvolveMenu, CGameObject)

HRESULT CUI_EvolveMenu::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pLeftBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	
	m_pLeftBG.lock()->Set_UIPosition(223.f, 150.f + (g_iWinCY / 2.f), 480.f, 1200.f);
	m_pLeftBG.lock()->Set_Depth(0.3f);
	m_pLeftBG.lock()->Set_Texture("EvolveMenu_LeftBG");


	Add_Child(m_pLeftBG);

	return S_OK;
}

void CUI_EvolveMenu::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_EvolveMenu::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

void CUI_EvolveMenu::Free()
{
}

