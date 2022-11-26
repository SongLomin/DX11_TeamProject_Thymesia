#include "stdafx.h"
#include "UI_EvolveMenu_Level.h"
#include "UI_Containers.h"
#include "UI_Elements.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "UI_EvolveMenu.h"

GAMECLASS_C(CUI_EvolveMenu_Level)
CLONE_C(CUI_EvolveMenu_Level, CGameObject)


HRESULT CUI_EvolveMenu_Level::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iArrowArraySize = 11;

	m_fDecorationArrowPos[0] = { 594.f ,184.f };
	m_fDecorationArrowPos[1] = { 594.f ,252.f};
	m_fDecorationArrowPos[2] = { 594.f ,307.f};
	m_fDecorationArrowPos[3] = { 631.f ,475.f};
	m_fDecorationArrowPos[4] = { 631.f ,552.f};
	m_fDecorationArrowPos[5] = { 631.f ,629.f};
	m_fDecorationArrowPos[6] = { 1300.f ,246.f };
	m_fDecorationArrowPos[7] = { 1300.f ,310.f };
	m_fDecorationArrowPos[8] = { 1300.f ,386.f };
	m_fDecorationArrowPos[9] = { 1300.f ,456.f };
	m_fDecorationArrowPos[10] = {1300.f ,526.f };

	

	m_tTextInfo_Memory.bAlways = false;
	m_tTextInfo_Memory.bCenterAlign = true;
	m_tTextInfo_Memory.fRotation = 0.f;
	m_tTextInfo_Memory.szText = L"11";
	m_tTextInfo_Memory.vPosition = _float2(544.f, 191.f - 8.f);
	m_tTextInfo_Memory.vScale = _float2(1.7f, 1.7f);
	m_tTextInfo_Memory.vColor = _float4(1.f, 1.f, 1.f, 1.0f);


	Create_Background();
	Create_Memory();
	Create_Status();
	Create_State();
	Create_NoneGrouping();


	m_iSelectedIndex = 0;

	ChageSelectedIndex();

	Set_Enable(false);
    return S_OK;
}

void CUI_EvolveMenu_Level::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY_INPUT(KEY::T, KEY_STATE::TAP))
	{
		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 0.2f;
		tFaderDesc.fDelayTime = 0.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_EvolveMenu_Level::Call_ReturnToEvolveMenu, this);
	}


	if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
	{
		if (m_iSelectedIndex > 0)
		{
			m_iSelectedIndex--;
			ChageSelectedIndex();
		}
	}
	if (KEY_INPUT(KEY::DOWN, KEY_STATE::TAP))
	{
		if (m_iSelectedIndex <= (_uint)EVOLVE_LEVEL_TYPE::PLA)
		{
			m_iSelectedIndex++;
			ChageSelectedIndex();
		}
	}
}

void CUI_EvolveMenu_Level::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::HEIROLIGHT, m_tTextInfo_Memory);

}

void CUI_EvolveMenu_Level::Create_Background()
{
	m_pPauseMenuBackground = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground.lock()->Set_Texture("PauseMenu_Background0");
	m_pPauseMenuBackground.lock()->Set_Depth(0.9f);

	m_pPauseMenuBackground_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground_Main.lock()->Set_Texture("PauseMenu_Background1");
	m_pPauseMenuBackground_Main.lock()->Set_Depth(0.8f);

	m_pPauseMenuBackground_Top = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground_Top.lock()->Set_Texture("PauseMenu_Background3");
	m_pPauseMenuBackground_Top.lock()->Set_Depth(0.7f);


	m_vecChildUI.push_back(m_pPauseMenuBackground);
	m_vecChildUI.push_back(m_pPauseMenuBackground_Main);
	m_vecChildUI.push_back(m_pPauseMenuBackground_Top);
}

void CUI_EvolveMenu_Level::Create_Memory()
{
	m_pTitle = ADD_STATIC_CUSTOMUI;
	m_pTitle.lock()->Set_UIPosition
	(
		54.f,
		38.f,
		474.f,
		64.f,
		ALIGN_LEFTTOP
	);
	m_pTitle.lock()->Set_Depth(0.f);
	m_pTitle.lock()->Set_Texture("EvolveMenu_LevelUp_Title");

	m_pLevel = ADD_STATIC_CUSTOMUI;
	m_pLevel.lock()->Set_UIPosition
	(
		144.f,
		184.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP

	);
	m_pLevel.lock()->Set_Texture("EvolveMenu_LevelUp_Level");

	m_pMemory = ADD_STATIC_CUSTOMUI;
	m_pMemory.lock()->Set_UIPosition
	(
		144.f,
		246.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pMemory.lock()->Set_Texture("EvolveMenu_LevelUp_Memory");


	m_pRequired_Memory = ADD_STATIC_CUSTOMUI;
	m_pRequired_Memory.lock()->Set_UIPosition
	(
		144.f,
		300.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pRequired_Memory.lock()->Set_Texture("EvolveMenu_LevelUp_Required_Memory");


	Add_Child(m_pTitle);
	Add_Child(m_pLevel);
	Add_Child(m_pMemory);
	Add_Child(m_pRequired_Memory);

}

void CUI_EvolveMenu_Level::Create_Status()
{
	m_pStatusBG = ADD_STATIC_CUSTOMUI;

	m_pStatusBG.lock()->Set_UIPosition
	(
		97.f,
		397.f,
		770.f,
		296.f,
		ALIGN_LEFTTOP
	);
	m_pStatusBG.lock()->Set_Texture("EvolveMenu_LevelUp_StatusBG");
	m_pStatusBG.lock()->Set_PassIndex(1);
	m_pStatusBG.lock()->Set_Depth(0.6f);

	m_pStatusTitle = ADD_STATIC_CUSTOMUI;
	
	m_pStatusTitle.lock()->Set_UIPosition
	(
		144.f,
		407.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pStatusTitle.lock()->Set_Texture("EvolveMenu_LevelUp_Status_Title");

	m_pStatusStrength = ADD_STATIC_CUSTOMUI;

	m_pStatusStrength.lock()->Set_UIPosition
	(
		151.f,
		467.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pStatusStrength.lock()->Set_Texture("EvolveMenu_LevelUp_Status_Strength");
	m_pStatusStrength.lock()->Set_AlphaColor(0.7f);


	m_pStatusVitality = ADD_STATIC_CUSTOMUI;
	
	m_pStatusVitality.lock()->Set_UIPosition
	(
		151.f,
		544.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pStatusVitality.lock()->Set_Texture("EvolveMenu_LevelUp_Status_Vitality");
	m_pStatusVitality.lock()->Set_AlphaColor(0.7f);

	m_pStatusPlague = ADD_STATIC_CUSTOMUI;

	m_pStatusPlague.lock()->Set_UIPosition
	(
		151.f,
		621.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pStatusPlague.lock()->Set_Texture("EvolveMenu_LevelUp_Status_Plague");
	m_pStatusPlague.lock()->Set_AlphaColor(0.7f);


	Add_Child(m_pStatusBG);
	Add_Child(m_pStatusTitle);
	Add_Child(m_pStatusStrength);
	Add_Child(m_pStatusVitality);
	Add_Child(m_pStatusPlague);


}

void CUI_EvolveMenu_Level::Create_State()
{
	m_pState_Title = ADD_STATIC_CUSTOMUI;
	m_pState_Title.lock()->Set_UIPosition
	(
		913.f,
		178.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pState_Title.lock()->Set_Texture("EvolveMenu_LevelUp_State_Title");
	m_pState_Title.lock()->Set_Depth(0.5f);

	m_pState_AttackDamage = ADD_STATIC_CUSTOMUI;
	m_pState_AttackDamage.lock()->Set_UIPosition
	(
		949.f,
		238.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pState_AttackDamage.lock()->Set_Texture("EvolveMenu_LevelUp_State_AttackDamgage");
	m_pState_AttackDamage.lock()->Set_AlphaColor(0.7f);
	m_pState_AttackDamage.lock()->Set_Depth(0.5f);


	m_pState_Wound = ADD_STATIC_CUSTOMUI;
	m_pState_Wound.lock()->Set_UIPosition
	(
		949.f,
		308.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pState_Wound.lock()->Set_Texture("EvolveMenu_LevelUp_State_Wound");
	m_pState_Wound.lock()->Set_AlphaColor(0.7f);
	m_pState_Wound.lock()->Set_Depth(0.5f);


	m_pState_ClawDamage = ADD_STATIC_CUSTOMUI;
	m_pState_ClawDamage.lock()->Set_UIPosition
	(
		949.f,
		378.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pState_ClawDamage.lock()->Set_Texture("EvolveMenu_LevelUp_State_ClawDamage");
	m_pState_ClawDamage.lock()->Set_AlphaColor(0.7f);
	m_pState_ClawDamage.lock()->Set_Depth(0.5f);

	m_pState_HP = ADD_STATIC_CUSTOMUI;
	m_pState_HP.lock()->Set_UIPosition
	(
		949.f,
		448.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pState_HP.lock()->Set_Texture("EvolveMenu_LevelUp_State_HP");
	m_pState_HP.lock()->Set_AlphaColor(0.7f);
	m_pState_HP.lock()->Set_Depth(0.5f);


	m_pState_MP = ADD_STATIC_CUSTOMUI;
	m_pState_MP.lock()->Set_UIPosition
	(
		949.f,
		518.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pState_MP.lock()->Set_Texture("EvolveMenu_LevelUp_State_MP");
	m_pState_MP.lock()->Set_AlphaColor(0.7f);
	m_pState_MP.lock()->Set_Depth(0.5f);


	Add_Child(m_pState_Title);
	Add_Child(m_pState_AttackDamage);
	Add_Child(m_pState_Wound);
	Add_Child(m_pState_ClawDamage);
	Add_Child(m_pState_HP);
	Add_Child(m_pState_MP);
}

void CUI_EvolveMenu_Level::Create_NoneGrouping()
{
	m_pApply = ADD_STATIC_CUSTOMUI;

	m_pApply.lock()->Set_UIPosition
	(
		459.f,
		726.f,
		86.f,
		36.f,
		ALIGN_LEFTTOP
	);
	m_pApply.lock()->Set_Texture("EvolveMenu_LevelUp_Apply");
	m_pApply.lock()->Set_Depth(0.5f);

	m_pHighlight = ADD_STATIC_CUSTOMUI;

	m_pHighlight.lock()->Set_UIPosition
	(
		121.f,
		725.f,
		723.f,
		39.f,
		ALIGN_LEFTTOP
	);
	m_pHighlight.lock()->Set_Texture("MainMenu_SelectableButton_2");
	m_pHighlight.lock()->Set_Depth(0.6f);



	weak_ptr<CCustomUI>	pArrow;

	for (_uint i = 0; i < m_iArrowArraySize; i++)
	{
		pArrow = ADD_STATIC_CUSTOMUI;
		pArrow.lock()->Set_UIPosition(
			m_fDecorationArrowPos[i].x,
			m_fDecorationArrowPos[i].y, 
			52.f, 
			23.f, 
			ALIGN_LEFTTOP);
		pArrow.lock()->Set_Depth(0.4f);
		pArrow.lock()->Set_Texture("Decoration_Arrow_Right");
		pArrow.lock()->Set_AlphaColor(0.6f);
		Add_Child(pArrow);
	}
	
	Add_Child(m_pHighlight);
	Add_Child(m_pApply);

}


void CUI_EvolveMenu_Level::ChageSelectedIndex()
{
	EVOLVE_LEVEL_TYPE eType = (EVOLVE_LEVEL_TYPE)m_iSelectedIndex;
	
	/*
	m_fSelectedIndexPosArray[(_uint)EVOLVE_LEVEL_TYPE::STR] = _float2(231.f, 465.f);
	m_fSelectedIndexPosArray[(_uint)EVOLVE_LEVEL_TYPE::VIT] = _float2(231.f, 542.f);
	m_fSelectedIndexPosArray[(_uint)EVOLVE_LEVEL_TYPE::PLA] = _float2(231.f, 619.f);
	
	*/

	switch (eType)
	{
	case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::STR:
		m_pHighlight.lock()->Set_UIPosition(231.f, 465.f, ALIGN_LEFTTOP);
		break;
	case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::VIT:
		m_pHighlight.lock()->Set_UIPosition(231.f, 542.f, ALIGN_LEFTTOP);
		break;
	case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::PLA:
		m_pHighlight.lock()->Set_UIPosition(231.f, 619.f, ALIGN_LEFTTOP);
		break;
	case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::APPLY:
		m_pHighlight.lock()->Set_UIPosition(121.f, 725.f, ALIGN_LEFTTOP);
		break;
	default:
		break;
	}

}

void CUI_EvolveMenu_Level::SelectButton()
{
}

void CUI_EvolveMenu_Level::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	if (!m_pFadeMask.lock())
		m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::BATTLEUI);

}

void CUI_EvolveMenu_Level::Call_ReturnToEvolveMenu()
{
	Set_Enable(false);
	GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu>(LEVEL_STATIC).front().lock()->Set_Enable(true);

}
