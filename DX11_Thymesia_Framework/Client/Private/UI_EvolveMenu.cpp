#include "stdafx.h"
#include "UI_EvolveMenu.h"
#include "UI_Elements.h"



GAMECLASS_C(CUI_EvolveMenu)
CLONE_C(CUI_EvolveMenu, CGameObject)

HRESULT CUI_EvolveMenu::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iSelectedIndex = 0;

	ZeroMemory(m_bOpenedPage, sizeof(_bool) * (_uint)EVOLOVEMENU_TYPE::EVOLVE_END);

	m_bOpenedPage[(_uint)EVOLOVEMENU_TYPE::EVOLVE_LEVELUP] = true;
	

	strcpy_s(m_MenuTextKey[(_uint)EVOLOVEMENU_TYPE::EVOLVE_LEVELUP], "EvolveMenu_Text_LevelUp");
	strcpy_s(m_MenuTextKey[(_uint)EVOLOVEMENU_TYPE::EVOLVE_UNLOCKTALENT], "EvolveMenu_Text_UnlockTalent");
	strcpy_s(m_MenuTextKey[(_uint)EVOLOVEMENU_TYPE::EVOLVE_PLAGUEWEAPON], "EvolveMenu_Text_PlagueWeapon");
	strcpy_s(m_MenuTextKey[(_uint)EVOLOVEMENU_TYPE::EVOLVE_POTION], "EvolveMenu_Text_Potion");
	strcpy_s(m_MenuTextKey[(_uint)EVOLOVEMENU_TYPE::EVOLVE_FEATHER], "EvolveMenu_Text_UseForgottenFeather");
	
#pragma region CREATE_UIS

	m_pLeftBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pLeftBG.lock()->Set_UIPosition(223.f, 150.f + (g_iWinCY / 2.f), 480.f, 1200.f);
	m_pLeftBG.lock()->Set_Depth(0.3f);
	m_pLeftBG.lock()->Set_Texture("EvolveMenu_LeftBG");

	Add_Child(m_pLeftBG);


	m_pSelectHighlight = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pSelectHighlight.lock()->Set_UIPosition(600 * 0.5f, 150.f + (g_iWinCY / 2.f), 600, 35.f);
	m_pSelectHighlight.lock()->Set_Depth(0.1f);
	m_pSelectHighlight.lock()->Set_Texture("EvolveMenu_Text_SelectHighlight");


	Add_Child(m_pSelectHighlight);
	

#pragma endregion CREATE_UIS




#pragma region CREATE_TEXTUI

	UI_DESC tTextDesc;

	
	tTextDesc.fSizeY = 34.f;
	tTextDesc.fDepth = 0.f;

	for (_uint i = 0; i < (_uint)EVOLOVEMENU_TYPE::EVOLVE_END; i++)
	{
		if (i == (_uint)EVOLOVEMENU_TYPE::EVOLVE_FEATHER)
			tTextDesc.fSizeX = 180.f;
		else
			tTextDesc.fSizeX = 103.f;


		tTextDesc.fX = 143.f + tTextDesc.fSizeX * 0.5f;
		tTextDesc.fY = 221.f + (68.f * (_float)i);

		m_pMenuText[i] = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &tTextDesc);
		m_pMenuText[i].lock()->Set_Texture(m_MenuTextKey[i]);
		Add_Child(m_pMenuText[i]);
	}
#pragma endregion CREATE_TEXTUI



	ChangeButtonIndex();

	return S_OK;
}

void CUI_EvolveMenu::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
	{
		if (m_iSelectedIndex <= 0)
			m_iSelectedIndex = (_uint)EVOLOVEMENU_TYPE::EVOLVE_END - 1;
		else
			m_iSelectedIndex--;
		ChangeButtonIndex();
	}

	else if (KEY_INPUT(KEY::DOWN, KEY_STATE::TAP))
	{
		if (m_iSelectedIndex >= (_uint)EVOLOVEMENU_TYPE::EVOLVE_END - 1)
			m_iSelectedIndex = 0;
		else
			m_iSelectedIndex++;

		ChangeButtonIndex();
	}

	ChangeButtonIndex();
}

void CUI_EvolveMenu::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

void CUI_EvolveMenu::Free()
{
}

void CUI_EvolveMenu::ChangeButtonIndex()
{
	m_pSelectHighlight.lock()->Set_Y(m_pMenuText[m_iSelectedIndex].lock()->Get_UIDESC().fY);

}

