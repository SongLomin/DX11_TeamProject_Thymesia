#include "stdafx.h"
#include "ClientLevel.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "GameManager.h"
#include "EffectGroup.h"
#include "UI_PauseMenu.h"
#include "UI_EvolveMenu.h"
#include "HUD_PlagueWeapon.h"
#include "UI_Landing.h"
#include "UIManager.h"
#include "Static_Instancing_Prop.h"
#include "HUD_PlagueWeapon.h"
#include "UI_PauseMenu.h"
#include "MonsterHPBar_Base.h"
#include "MonsterHPBar_Elite.h"
#include "MonsterHPBar_Boss.h"
#include "MonsterParryingBar.h"
#include "UI_Containers.h"
#include "UI_EvolveMenu.h"
#include "UI_EvolveMenu_Level.h"
#include "UI_EvolveMenu_Talent.h"
#include "Player_MPBar.h"
#include "Player_HPBar.h"
#include "HUD_Player_Memory.h"
#include "Player_PotionUI.h"
#include "Player_FeatherUI.h"
#include "UI_Script.h"
#include "UI_ScriptQueue.h"
#include "UI_DamageFont.h"
#include "TestUI.h"
#include "UI_EvolveMenu_SelectDecoration.h"
#include "UI_Interaction.h"
#include "UI_Utils.h"
#include "UI_MonsterFocus.h"
#include "UI_Cursor.h"
#include "UI_ItemPopup.h"
#include "UI_BloodOverlay.h"
#include "UI_AppearEventVarg.h"
#include "UI_ItemRequirement.h"
#include "UI_EvolveMenu_PlagueWeapon.h"
#include "UI_EvolveMenu_Option.h"

GAMECLASS_C(CClientLevel)

HRESULT CClientLevel::Initialize()
{
	__super::Initialize();


	m_bLading = false;

	GAMEINSTANCE->Set_CreatedLevelIndex(m_eMyLevel);

	return S_OK;
}

void CClientLevel::Load_FromJson(const string& In_szJsonPath, const LEVEL& In_eLevel)
{
	json LoadedJson;

	if (FAILED(CJson_Utility::Load_Json(In_szJsonPath.c_str(), LoadedJson)))
	{
		//DEBUG_ASSERT;
	}

	for (auto& Elem_GameObjects : LoadedJson["GameObject"])
	{
		weak_ptr<CGameObject> pGameObjectInstance = GAMEINSTANCE->Add_GameObject(Elem_GameObjects["Hash"], (_uint)In_eLevel);

		// TODO : 객체가 생성되지 않음. Loader에서 프로토타입이 생성됐는지 확인 필요함.

#ifdef _DEBUG_COUT_
		if (!pGameObjectInstance.lock().get())
			cout << "객체가 생성되지 않음. 프로토타입 확인" << (string)Elem_GameObjects["Name"] << endl;
#endif// _DEBUG_COUT_

		if (pGameObjectInstance.lock().get())
		{
			pGameObjectInstance.lock()->Set_Enable(Elem_GameObjects["Setting"]["Enable"]);
			pGameObjectInstance.lock()->Load_FromJson(Elem_GameObjects);
		}
	}
}

void CClientLevel::Loading_AllEffectGroup(const char* In_FolderPath, const _uint& In_LevelIndex)
{
	std::filesystem::path dir_path = In_FolderPath;
	std::vector<fs::directory_entry> entries;
	std::copy(fs::directory_iterator(dir_path), fs::directory_iterator(), std::back_inserter(entries));
	fs::directory_iterator itr(dir_path);
	std::filesystem::directory_iterator end;

	/*for_each_n(std::execution::par, entries.begin(), entries.size(), [In_LevelIndex](const std::filesystem::directory_entry& entry)
		{
			weak_ptr<CEffectGroup> EffectGroup = GAMEINSTANCE->Add_GameObject<CEffectGroup>(In_LevelIndex);
			EffectGroup.lock()->Load_EffectJson(entry.path().string(), (_uint)TIMESCALE_LAYER::NONE, In_LevelIndex);
			cout << entry.path().filename() << std::endl;
		});*/

	

	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;

		weak_ptr<CEffectGroup> EffectGroup = GAMEINSTANCE->Add_GameObject<CEffectGroup>(In_LevelIndex);
		EffectGroup.lock()->Load_EffectJson(entry.path().string(), (_uint)TIMESCALE_LAYER::NONE, In_LevelIndex);
#ifdef _DEBUG_COUT_
		cout << entry.path().filename() << std::endl;
#endif

		itr++;
	}
}

void CClientLevel::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//한번만 실행되도록
	/*
		다른 실행시점에서 테스트해보니, 이니셜라이즈나 스타트에서
		세팅해주는게 많아서 정상적으로 작동안함
		그래서 Tick에서 한번만 실행되도록 추가해줌.
	*/
	if (!m_bLading)
	{
		m_bLading = true;
		Call_StageLanding();
	}
}

void CClientLevel::SetUp_UI()
{
	weak_ptr<CGameManager>	pGameManager = GET_SINGLE(CGameManager);

	weak_ptr<CUIManager>	pUIManager = GET_SINGLE(CUIManager);

	pUIManager.lock()->SetCursor(GAMEINSTANCE->Add_SingleGameObject<CUI_Cursor>(LEVEL_STATIC));

	m_pPauseMenu = GAMEINSTANCE->Add_SingleGameObject<CUI_PauseMenu>(LEVEL_STATIC);
	m_pEvolveMenu = GAMEINSTANCE->Add_SingleGameObject<CUI_EvolveMenu>(LEVEL_STATIC);
	GAMEINSTANCE->Add_SingleGameObject<CUI_EvolveMenu_PlagueWeapon>(LEVEL_STATIC);
	GAMEINSTANCE->Add_SingleGameObject<CUI_EvolveMenu_Level>(LEVEL_STATIC);
	GAMEINSTANCE->Add_SingleGameObject<CUI_EvolveMenu_Option>(LEVEL_STATIC);

	Preset::AddGameObject::TalentSetting();

	pGameManager.lock()->Register_Layer(OBJECT_LAYER::PLAYERHUD, GAMEINSTANCE->Add_SingleGameObject<CPlayer_HPBar>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::PLAYERHUD, GAMEINSTANCE->Add_SingleGameObject<CPlayer_MPBar>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::PLAYERHUD, GAMEINSTANCE->Add_SingleGameObject<CHUD_Player_Memory>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::PLAYERHUD, GAMEINSTANCE->Add_SingleGameObject<CHUD_PlagueWeapon>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::PLAYERHUD, GAMEINSTANCE->Add_SingleGameObject<CPlayer_PotionUI>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::EVENT_UI, GAMEINSTANCE->Add_SingleGameObject<CUI_ScriptQueue>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::INTERACTIONUI, GAMEINSTANCE->Add_GameObject<CUI_Interaction>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_SingleGameObject<CUI_MonsterFocus>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::EVENT_UI, GAMEINSTANCE->Add_SingleGameObject<CUI_AppearEventVarg>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::EVENT_UI, GAMEINSTANCE->Add_SingleGameObject<CUI_Landing>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::EVENT_UI, GAMEINSTANCE->Add_SingleGameObject<CUI_ItemRequirement>(LEVEL_STATIC));

	GAMEINSTANCE->Add_SingleGameObject<CUI_BloodOverlay>(LEVEL_STATIC);

	pUIManager.lock()->CreateItemPopupQueue();



	//pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CEvolveMenu_TalentButton>(LEVEL_STATIC));
	//TODO : MonsterHpBar TestCode
	/*
	CUI::UI_DESC tDesc;
	tDesc.fX = g_iWinCX / 2.f;
	tDesc.fY = g_iWinCY / 2.f;
	tDesc.fSizeX = 150.f;
	tDesc.fSizeY = 15.f;
	tDesc.fDepth = 0.f;

	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CMonsterHPBar_Elite>(LEVEL_STATIC, &tDesc));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CMonsterHPBar_Boss>(LEVEL_STATIC, &tDesc));
	*/

}

void CClientLevel::Tick_Key_InputEvent()
{
	if (KEY_INPUT(KEY::CTRL, KEY_STATE::TAP))
	{
		if (m_pPauseMenu.lock()->Get_Enable() == false)
		{
			FaderDesc tFaderDesc;
			tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
			tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
			tFaderDesc.fFadeMaxTime = 0.3f;
			tFaderDesc.fDelayTime = 0.f;
			tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

			m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
			m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_Enable_PauseMenu, this);

		}
	}

	if (!m_bFadeTrigger)
	{
		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_IN;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 3.f;
		tFaderDesc.fDelayTime = 0.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.f);

		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_FadeOutToStartGame, this);
		m_bFadeTrigger = true;
	}
}

void CClientLevel::Call_StageLanding()
{	
	if (m_eMyLevel == LEVEL_LOADING || m_eMyLevel == LEVEL_LOGO || m_eMyLevel == LEVEL_EDIT)
		return;

	weak_ptr<CUI_Landing> pUILanding = GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front();
		
	if (!pUILanding.lock())
	{
		return;
	}
	pUILanding.lock()->Call_Landing(
		CUI_Landing::LANDING_ENTER_STAGE
	);
}

void CClientLevel::Change_NextLevel(void* pArg)
{
	m_bChangeNextLevel = true;
}

void CClientLevel::Call_FadeOutToLevelChange()
{
	//weak_ptr<CGameObject> temp = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::FADEMASK).front();
	//m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

	if (m_bFadeOut)
		return;

	m_bFadeOut = true;

	m_pFadeMask.lock()->CallBack_FadeEnd -= bind(&CClientLevel::Call_FadeOutToLevelChange, this);
	m_pFadeMask.lock()->Set_Enable(false);
	if (FAILED(GAMEINSTANCE->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_eNextLevel))))
		return;
}

void CClientLevel::OnLevelEnter()
{
	__super::OnLevelEnter();

	if (m_eMyLevel == LEVEL_LOGO || m_eMyLevel == LEVEL_LOADING)
		return;

	Call_StageLanding();
}

void CClientLevel::ExitLevel(LEVEL eLevel)
{
	if (m_eMyLevel != LEVEL_LOGO)
	{
		GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Save_ClientComponentData();
	}
}

void CClientLevel::Call_Enable_PauseMenu()
{



	m_pPauseMenu.lock()->Set_Enable(true);
	m_pFadeMask.lock()->Set_Enable(false);
}

void CClientLevel::Call_Enable_EvolveMenu()
{
	m_pEvolveMenu.lock()->Set_Enable(true);
	m_pFadeMask.lock()->Set_Enable(false);
}

void CClientLevel::Call_FadeOutToStartGame()
{
	m_pFadeMask.lock()->Set_Enable(false);
	GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::PLAYERHUD);
}

void CClientLevel::Free()
{

}
