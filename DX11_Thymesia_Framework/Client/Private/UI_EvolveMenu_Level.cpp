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


	//Create_Background();
	Create_Memory();
	Create_Status();
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
}

void CUI_EvolveMenu_Level::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

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

	m_pStatusVitality = ADD_STATIC_CUSTOMUI;
	
	m_pStatusVitality.lock()->Set_UIPosition
	(
		151.f,
		528.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pStatusVitality.lock()->Set_Texture("EvolveMenu_LevelUp_Status_Vitality");


	m_pStatusPlague = ADD_STATIC_CUSTOMUI;

	m_pStatusPlague.lock()->Set_UIPosition
	(
		151.f,
		589.f,
		200.f,
		34.f,
		ALIGN_LEFTTOP
	);
	m_pStatusPlague.lock()->Set_Texture("EvolveMenu_LevelUp_Status_Plague");


	Add_Child(m_pStatusBG);
	Add_Child(m_pStatusTitle);
	Add_Child(m_pStatusStrength);
	Add_Child(m_pStatusVitality);
	Add_Child(m_pStatusPlague);


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
