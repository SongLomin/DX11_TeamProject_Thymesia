#include "stdafx.h"
#include "UI_EvolveMenu.h"
#include "UI_Elements.h"
#include "Level.h"
#include "GameManager.h"
#include "ClientLevel.h"
#include "FadeMask.h"
#include "UI_EvolveMenu_Level.h"
#include "UI_EvolveMenu_Talent.h"
#include "UI_EvolveLeftBG.h"
#include "UI_EvolveMenu_SelectDecoration.h"
#include "Player.h"
#include "UIManager.h"
#include "UI_EvolveMenu_PlagueWeapon.h"
#include "ClientLevel.h"
#include "Interaction_CheckPoint.h"
#include "UI_EvolveMenu_Option.h"
#include "UI_EvolveMenu.h"


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
	//strcpy_s(m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_POTION], "EvolveMenu_Text_Potion");
	//strcpy_s(m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_FEATHER], "EvolveMenu_Text_UseForgottenFeather");
	strcpy_s(m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_CEASE_RECALL], "EvolveMenu_Text_Cease_Recall");
	strcpy_s(m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_RESUME_GAME], "EvolveMenu_Text_ResumeGame");

#pragma region CREATE_UIS
	//Left

	m_pLeftBG = GAMEINSTANCE->Add_GameObject<CUI_EvolveLeftBG>(LEVEL_STATIC);
	m_pLeftBG.lock()->Set_UIPosition(223.f, 150.f + (g_iWinCY / 2.f), 480.f, 1200.f);
	m_pLeftBG.lock()->Set_Depth(0.3f);
	Add_Child(m_pLeftBG);


	m_pSelectHighlight = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pSelectHighlight.lock()->Set_UIPosition(600 * 0.5f, 150.f + (g_iWinCY / 2.f), 600, 35.f);
	m_pSelectHighlight.lock()->Set_Depth(0.1f);
	m_pSelectHighlight.lock()->Set_Texture("EvolveMenu_Text_SelectHighlight");
	Add_Child(m_pSelectHighlight);
	

	m_pLeftSelectDecoration = GAMEINSTANCE->Add_GameObject<CUI_EvolveMenu_SelectDecoration>(LEVEL_STATIC);
	m_pLeftSelectDecoration.lock()->Set_Depth(0.0f);
	Add_Child(m_pLeftSelectDecoration);


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
	m_pRightBG.lock()->Set_UIPosition(875.f, 70.f, 707.f, 665.f, ALIGN_LEFTTOP);
	m_pRightBG.lock()->Set_Texture("EvolveMenu_RightBG");
	m_pRightBG.lock()->Set_Depth(0.3f);
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
		895.f,
		120.f,
		660.f,
		320.f,
		CUI::ALIGN_LEFTTOP
	);
	m_pRightMapImage.lock()->Set_Texture("EvolveMenu_MapImage_SeaOfTrees");
	m_pRightMapImage.lock()->Set_PassIndex(8);
	Add_Child(m_pRightMapImage);

	m_pQuestTitle = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pQuestTitle.lock()->Set_UIPosition
	(
		955.f,
		461.f,
		53.f,
		34.f,
		CUI::ALIGN_LEFTTOP
	);
	m_pQuestTitle.lock()->Set_Texture("EvolveMenu_QuestTitle");
	
	Add_Child(m_pQuestTitle);

	m_pQuestTitleDecoration = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pQuestTitleDecoration.lock()->Set_UIPosition
	(
		960,
		491.f,
		152.f,
		4.f,
		CUI::ALIGN_LEFTTOP
	);
	m_pQuestTitleDecoration.lock()->Set_Texture("HUD_Player_Memory_Decoration");
	m_pQuestTitleDecoration.lock()->Set_AlphaColor(0.3f);

	Add_Child(m_pQuestTitleDecoration);

	m_pQuestInformation = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pQuestInformation.lock()->Set_Texture("EvolveMenu_Stage1_Quest1");
	m_pQuestInformation.lock()->Set_UIPosition
	(
		1014.f,
		519.f,
		494.f,
		28.f,
		CUI::ALIGN_LEFTTOP
	);
	Add_Child(m_pQuestInformation);


	m_pQuestInformationsDecoration = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pQuestInformationsDecoration.lock()->Set_Texture("Font_Diamond");
	m_pQuestInformationsDecoration.lock()->Set_UIPosition
	(
		994.f,
		524.f,
		14.f,
		14.f,
		CUI::ALIGN_LEFTTOP
	);
	Add_Child(m_pQuestInformationsDecoration);

	
	SetUpFromCurrentLevel();


#pragma endregion CREATE_UIS


#pragma region CREATE_TEXTUI

	UI_DESC tTextDesc;

	
	tTextDesc.fSizeY = 34.f;
	tTextDesc.fDepth = 0.f;

	for (_uint i = 0; i < (_uint)EVOLVEMENU_TYPE::EVOLVE_END; i++)
	{
		//if (i == (_uint)EVOLVEMENU_TYPE::EVOLVE_FEATHER)
		//	tTextDesc.fSizeX = 180.f;
		if (i == (_uint)EVOLVEMENU_TYPE::EVOLVE_CEASE_RECALL)
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

	
	SetUpFromCurrentLevel();
	m_bEnabledThisFrame = false;

	Set_Enable(false);

	return S_OK;
}

void CUI_EvolveMenu::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
	{
		GAMEINSTANCE->PlaySound2D("UI_ChangeIndex0.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT));


		if (m_iSelectedIndex <= 0)
			m_iSelectedIndex = (_uint)EVOLVEMENU_TYPE::EVOLVE_END - 1;
		else
			m_iSelectedIndex--;
		ChangeButtonIndex();
	}

	else if (KEY_INPUT(KEY::DOWN, KEY_STATE::TAP))
	{
		GAMEINSTANCE->PlaySound2D("UI_ChangeIndex0.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT));


		if (m_iSelectedIndex >= (_uint)EVOLVEMENU_TYPE::EVOLVE_END - 1)
			m_iSelectedIndex = 0;
		else
			m_iSelectedIndex++;

		ChangeButtonIndex();
	}
	if (KEY_INPUT(KEY::ENTER, KEY_STATE::TAP))
	{
		GAMEINSTANCE->PlaySound2D("Fantasy_Game_UI_Ice_Select.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT));
		SelectButton();
	}
	m_bEnabledThisFrame = false;
	
}

void CUI_EvolveMenu::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

void CUI_EvolveMenu::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);
	
	m_bEnabledThisFrame = true;
	SetUpFromCurrentLevel();
	if (!m_pFadeMask.lock())
		m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();
	
	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::PLAYERHUD);
	ChangeButtonIndex();

	GET_SINGLE(CGameManager)->Set_MoveTargetCamera(false);

}

void CUI_EvolveMenu::OnDisable()
{
	__super::OnDisable();

	GET_SINGLE(CUIManager)->Set_OpenedMenu(false);

	GET_SINGLE(CGameManager)->Set_MoveTargetCamera(true);

	weak_ptr<CPlayer> pPlayer;

	pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (pPlayer.lock())
	{
		pPlayer.lock()->Get_Component<CStatus_Player>().lock()->Full_Recovery();
	}


}

void CUI_EvolveMenu::Call_FadeEndEnableEvolveMenu()
{
	GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::PLAYERHUD);
	Set_Enable(false);
	m_pFadeMask.lock()->Set_Enable(false);

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	pCurrentPlayer.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_SITUP);
}


void CUI_EvolveMenu::Call_ChangeUI_EvolveMenu_Level()
{
	Set_Enable(false);
	m_pFadeMask.lock()->Set_Enable(false);

	GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu_Level>(LEVEL_STATIC).front().lock()->Set_Enable(true);

}

void CUI_EvolveMenu::Call_ChangeUI_EvolveMenu_Talent()
{
	Set_Enable(false);
	m_pFadeMask.lock()->Set_Enable(false);

	GAMEINSTANCE->Get_GameObjects<CUI_EveolveMenu_Talent>(LEVEL_STATIC).front().lock()->Set_Enable(true);
}

void CUI_EvolveMenu::Call_ChangeUI_EvolveMenu_PlagueWeapon()
{
	Set_Enable(false);
	m_pFadeMask.lock()->Set_Enable(false);

	GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu_PlagueWeapon>(LEVEL_STATIC).front().lock()->Set_Enable(true);
}
void CUI_EvolveMenu::Call_ChangeUI_EvolveMenu_Option()
{
	Set_Enable(false);
	m_pFadeMask.lock()->Set_Enable(false);

	GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu_Option>(LEVEL_STATIC).front().lock()->Set_Enable(true);
}
void CUI_EvolveMenu::Free()
{

}

void CUI_EvolveMenu::ChangeButtonIndex()
{
	m_pSelectHighlight.lock()->Set_Y(m_pMenuText[m_iSelectedIndex].lock()->Get_UIDESC().fY);
	m_pLeftSelectDecoration.lock()->Start_Animation();

	_float2		fDecorationPos;

	fDecorationPos = m_pSelectHighlight.lock()->Get_Point(UI_POINT::LEFT);

	m_pLeftSelectDecoration.lock()->Set_UIPosition
	(
		fDecorationPos.x + 32.f,
		fDecorationPos.y
	);
	for (_uint i = 0; i < (_uint)EVOLVEMENU_TYPE::EVOLVE_END; i++)
	{
		if (i == m_iSelectedIndex)
			m_pMenuText[i].lock()->Set_AlphaColor(1.f);
		else
			m_pMenuText[i].lock()->Set_AlphaColor(0.4f);
	}
}
void CUI_EvolveMenu::SetUpFromCurrentLevel()
{
	//82
	m_eLastOpenedLevel = Weak_StaticCast<CClientLevel>(GAMEINSTANCE->Get_CurrentLevel()).lock()->Get_MyLevel();

	UI_DESC		tRightBGDesc = m_pRightBG.lock()->Get_UIDESC();

	switch (m_eLastOpenedLevel)
	{
	case Client::LEVEL_GAMEPLAY:
		m_pRightTitle.lock()->Set_UIPosition(tRightBGDesc.fX, 82.f + 17.f, 100.f, 34.f);
		m_pRightTitle.lock()->Set_Texture("EvolveMenu_Text_SeaOfTrees");
		m_pRightMapImage.lock()->Set_Texture("EvolveMenu_MapImage_SeaOfTrees");
		break;
	case Client::LEVEL_TEST:
		m_pRightTitle.lock()->Set_UIPosition(tRightBGDesc.fX, 82.f + 17.f, 100.f, 34.f);
		m_pRightTitle.lock()->Set_Texture("EvolveMenu_Text_SeaOfTrees");
		m_pRightMapImage.lock()->Set_Texture("EvolveMenu_MapImage_SeaOfTrees");
		break;
	case Client::LEVEL_STAGE2:
		m_pRightTitle.lock()->Set_UIPosition(tRightBGDesc.fX, 82.f + 17.f, 100.f, 34.f);
		m_pRightTitle.lock()->Set_Texture("EvolveMenu_Text_RoyalGarden");
		m_pRightMapImage.lock()->Set_Texture("EvolveMenu_MapImage_Garden");
		m_pQuestInformation.lock()->Set_Texture("EvolveMenu_Stage2_Quest1");

		break;
	case Client::LEVEL_STAGE3:
		m_pRightTitle.lock()->Set_UIPosition(tRightBGDesc.fX, 82.f + 17.f, 100.f, 34.f);
		m_pRightTitle.lock()->Set_Texture("EvolveMenu_Text_HermessFortress");
		m_pRightMapImage.lock()->Set_Texture("EvolveMenu_MapImage_HermesFortress");
		m_pQuestInformation.lock()->Set_Texture("EvolveMenu_Stage3_Quest1");
		break;
	default:
		m_pRightTitle.lock()->Set_UIPosition(tRightBGDesc.fX, 82.f + 17.f, 100.f, 34.f);
		m_pRightTitle.lock()->Set_Texture("EvolveMenu_Text_SeaOfTrees");
		m_pRightMapImage.lock()->Set_Texture("EvolveMenu_MapImage_SeaOfTrees");
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
	FaderDesc tFaderDesc;

	switch (eType)
	{
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_LEVELUP:
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 0.3f;
		tFaderDesc.fDelayTime = 0.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_EvolveMenu::Call_ChangeUI_EvolveMenu_Level, this);
		break;
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_UNLOCKTALENT:
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 0.3f;
		tFaderDesc.fDelayTime = 0.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_EvolveMenu::Call_ChangeUI_EvolveMenu_Talent, this);
		break;
		break;
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_PLAGUEWEAPON:
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 0.3f;
		tFaderDesc.fDelayTime = 0.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_EvolveMenu::Call_ChangeUI_EvolveMenu_PlagueWeapon, this);
		break;
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_CEASE_RECALL:
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 0.3f;
		tFaderDesc.fDelayTime = 0.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_EvolveMenu::Call_ChangeUI_EvolveMenu_Option, this);
		break;
	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_RESUME_GAME:
	{
		//tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		//tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		//tFaderDesc.fFadeMaxTime = 0.3f;
		//tFaderDesc.fDelayTime = 0.f;
		//tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);
		//m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		//m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_EvolveMenu::Call_FadeEndEnableEvolveMenu, this);

		GET_SINGLE(CUIManager)->OnExitEvolveMenu();

		weak_ptr<CClientLevel> pClientLevel = Weak_StaticCast<CClientLevel>(GAMEINSTANCE->Get_CurrentLevel());
		LEVEL eCurrentLevel = pClientLevel.lock()->Get_MyLevel();

		list<weak_ptr<CInteraction_CheckPoint>> ObjList = GAMEINSTANCE->Get_GameObjects<CInteraction_CheckPoint>(eCurrentLevel);

		for (auto elem : ObjList)
			elem.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EXIT_SECTION);
	}
		break;

	case Client::CUI_EvolveMenu::EVOLVEMENU_TYPE::EVOLVE_END:
		break;
	default:
		break;
	}


}

