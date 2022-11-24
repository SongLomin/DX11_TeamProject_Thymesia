#include "stdafx.h"
#include "UI_EvolveMenu.h"
#include "UI_Elements.h"
#include "Level.h"
#include "GameManager.h"
#include "ClientLevel.h"


GAMECLASS_C(CUI_EvolveMenu)
CLONE_C(CUI_EvolveMenu, CGameObject)

HRESULT CUI_EvolveMenu::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iSelectedIndex = 0;
	
	ZeroMemory(m_bOpenedPage, sizeof(_bool) * (_uint)EVOLVEMENU_TYPE::EVOLVE_END);

	m_bOpenedPage[(_uint)EVOLVEMENU_TYPE::EVOLVE_LEVELUP] = true;
	

	strcpy_s(m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_LEVELUP], "EvolveMenu_Text_LevelUp");
	strcpy_s(m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_UNLOCKTALENT], "EvolveMenu_Text_UnlockTalent");
	strcpy_s(m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_PLAGUEWEAPON], "EvolveMenu_Text_PlagueWeapon");
	strcpy_s(m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_POTION], "EvolveMenu_Text_Potion");
	strcpy_s(m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_FEATHER], "EvolveMenu_Text_UseForgottenFeather");
	strcpy_s(m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_CEASE_RECALL], "EvolveMenu_Text_Cease_Recall");
	strcpy_s(m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_RESUME_GAME], "EvolveMenu_Text_ResumeGame");

#pragma region CREATE_UIS


	//Left

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
	

	m_pMenuTitleBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pMenuTitleBG.lock()->Set_UIPosition((465 * 0.5f), 106.f, 465.f, 62.f);
	m_pMenuTitleBG.lock()->Set_Depth(0.2f);
	m_pMenuTitleBG.lock()->Set_Texture("EvolveMenu_TitleBG");
	m_pMenuTitleBG.lock()->Set_AlphaColor(0.3f);
	Add_Child(m_pMenuTitleBG);

	m_pMenuTitleText = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pMenuTitleText.lock()->Set_UIPosition(76.f + 100.f, 106.f, 200.f, 58.f);
	m_pMenuTitleText.lock()->Set_Depth(0.1f);
	m_pMenuTitleText.lock()->Set_Texture("EvolveMenu_Text_Title_Eng");
	Add_Child(m_pMenuTitleText);


	//Right
	m_pRightBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pRightBG.lock()->Set_UIPosition(875.f, 70.f, 663.f, 660.f, ALIGN_LEFTTOP);
	m_pRightBG.lock()->Set_Texture("EvolveMenu_RightBG");

	Add_Child(m_pRightBG);

	m_pRightTitle = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pRightTitle.lock()->Set_Texture("EvolveMenu_Text_SeaOfTrees");
	Add_Child(m_pRightTitle);

	m_pRightTitleDecoration = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pRightTitleDecoration.lock()->Set_Texture("Font_Diamond");
	Add_Child(m_pRightTitleDecoration);


	m_pRightMapImage = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pRightMapImage.lock()->Set_UIPosition
	(
		934.f,
		130.f,
		567.f,
		313.f,
		CUI::ALIGN_LEFTTOP
	);
	m_pRightMapImage.lock()->Set_Texture("EvolveMenu_MapImage_SeaOfTrees");
	Add_Child(m_pRightMapImage);




#pragma endregion CREATE_UIS


#pragma region CREATE_TEXTUI

	UI_DESC tTextDesc;

	
	tTextDesc.fSizeY = 34.f;
	tTextDesc.fDepth = 0.f;

	for (_uint i = 0; i < (_uint)EVOLVEMENU_TYPE::EVOLVE_END; i++)
	{
		if (i == (_uint)EVOLVEMENU_TYPE::EVOLVE_FEATHER)
			tTextDesc.fSizeX = 180.f;
		else if (i == (_uint)EVOLVEMENU_TYPE::EVOLVE_CEASE_RECALL)
			tTextDesc.fSizeX = 200.f;
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
	ChangeUIFromCurrentLevel();
	m_bEnabledThisFrame = false;

	Set_Enable(false);

	return S_OK;
}

void CUI_EvolveMenu::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
	{
		if (m_iSelectedIndex <= 0)
			m_iSelectedIndex = (_uint)EVOLVEMENU_TYPE::EVOLVE_END - 1;
		else
			m_iSelectedIndex--;
		ChangeButtonIndex();
	}

	else if (KEY_INPUT(KEY::DOWN, KEY_STATE::TAP))
	{
		if (m_iSelectedIndex >= (_uint)EVOLVEMENU_TYPE::EVOLVE_END - 1)
			m_iSelectedIndex = 0;
		else
			m_iSelectedIndex++;

		ChangeButtonIndex();
	}
	if (KEY_INPUT(KEY::ENTER, KEY_STATE::TAP))
		SelectButton();


	m_bEnabledThisFrame = false;
	ChangeButtonIndex();
}

void CUI_EvolveMenu::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

void CUI_EvolveMenu::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);

	m_bEnabledThisFrame = true;
	ChangeUIFromCurrentLevel();

	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::BATTLEUI);
}

void CUI_EvolveMenu::Free()
{
}

void CUI_EvolveMenu::ChangeButtonIndex()
{
	m_pSelectHighlight.lock()->Set_Y(m_pMenuText[m_iSelectedIndex].lock()->Get_UIDESC().fY);


	for (_uint i = 0; i < (_uint)EVOLVEMENU_TYPE::EVOLVE_END; i++)
	{
		if (i == m_iSelectedIndex)
			m_pMenuText[i].lock()->Set_AlphaColor(1.f);
		else
			m_pMenuText[i].lock()->Set_AlphaColor(0.4f);
	}

}
void CUI_EvolveMenu::ChangeUIFromCurrentLevel()
{
	//82
	m_eLastOpenedLevel = Weak_StaticCast<CClientLevel>(GAMEINSTANCE->Get_CurrentLevel()).lock()->Get_MyLevel();


	UI_DESC		tRightBGDesc = m_pRightBG.lock()->Get_UIDESC();

	switch (m_eLastOpenedLevel)
	{
	case Client::LEVEL_GAMEPLAY:
		m_pRightTitle.lock()->Set_UIPosition(tRightBGDesc.fX, 82.f + 17.f, 103.f, 34.f);
		m_pRightTitle.lock()->Set_Texture("EvolveMenu_Text_SeaOfTrees");
		m_pRightMapImage.lock()->Set_Texture("EvolveMenu_MapImage_SeaOfTrees");
		break;
	case Client::LEVEL_TEST:
		m_pRightTitle.lock()->Set_UIPosition(tRightBGDesc.fX, 82.f + 17.f, 103.f, 34.f);
		m_pRightTitle.lock()->Set_Texture("EvolveMenu_Text_SeaOfTrees");
		break;
	default:
		m_pRightTitle.lock()->Set_UIPosition(tRightBGDesc.fX, 82.f + 17.f, 103.f, 34.f);
		m_pRightTitle.lock()->Set_Texture("EvolveMenu_Text_SeaOfTrees");
		break;
	}

	_float fTextFontLeft = m_pRightTitle.lock()->Get_Point(CUI::UI_POINT::LEFT).x;
	m_pRightTitleDecoration.lock()->Set_UIPosition
	(
		fTextFontLeft - 15.f,
		103.f - 7.f,
		14.f,
		14.f
	);
}

void CUI_EvolveMenu::SelectButton()
{
	CUI_EvolveMenu::EVOLVEMENU_TYPE eType = (CUI_EvolveMenu::EVOLVEMENU_TYPE)m_iSelectedIndex;

	switch (eType)
	{
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_LEVELUP:
		break;
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_UNLOCKTALENT:
		break;
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_PLAGUEWEAPON:
		break;
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_POTION:
		break;
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_FEATHER:
		break;
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_CEASE_RECALL:
		break;
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_RESUME_GAME:
		GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::BATTLEUI);
		Set_Enable(false);
		break;
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_END:
		break;
	default:
		break;
	}


}

