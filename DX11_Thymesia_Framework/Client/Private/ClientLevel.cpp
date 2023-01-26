#include "stdafx.h"
#include "ClientLevel.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "GameManager.h"
#include "EffectGroup.h"
#include "UIHeaders.h"
#include "Static_Instancing_Prop.h"
#include "SubThread_Pool.h"
#include "UI_Utils.h"
#include "UIManager.h"


GAMECLASS_C(CClientLevel)

HRESULT CClientLevel::Initialize()
{
	__super::Initialize();

	m_bLading = false;

	GAMEINSTANCE->Set_CreatedLevelIndex(m_eMyLevel);

	if (m_eMyLevel == LEVEL_LOGO)
	{

		_float fSound = GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_BGM);

		GAMEINSTANCE->PlayBGM("BGM_LOGO.ogg", fSound);
	}
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

#ifdef _OVERDRIVE_LOAD_EFFECTGROUP_
	GET_SINGLE(CGameManager)->Get_ClientThread()->Wait_JobDone();
#endif // _OVERDRIVE_LOAD_EFFECTGROUP_

	

	

	std::filesystem::path dir_path = In_FolderPath;
	std::vector<fs::directory_entry> entries;
	std::copy(fs::directory_iterator(dir_path), fs::directory_iterator(), std::back_inserter(entries));
	fs::directory_iterator itr(dir_path);
	std::filesystem::directory_iterator end;


	list<pair<string, json>> EffectJsons;

	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;

		weak_ptr<CEffectGroup> EffectGroup = GAMEINSTANCE->Add_GameObject<CEffectGroup>(In_LevelIndex);
		

#ifdef _OVERDRIVE_LOAD_EFFECTGROUP_
		GET_SINGLE(CGameManager)->Get_ClientThread()->Enqueue_Job(bind([entry, In_LevelIndex, EffectGroup]() {
			EffectGroup.lock()->Load_EffectJson(entry.path().string(), (_uint)TIMESCALE_LAYER::NONE, In_LevelIndex);
			}));
#else // _OVERDRIVE_LOAD_EFFECTGROUP_
		std::string strEntryPath = entry.path().string();
		EffectGroup.lock()->Load_EffectJson(strEntryPath, (_uint)TIMESCALE_LAYER::NONE, In_LevelIndex);
#endif // _OVERDRIVE_LOAD_EFFECTGROUP_

		

		itr++;
	}

#ifdef _OVERDRIVE_LOAD_EFFECTGROUP_
	GET_SINGLE(CGameManager)->Get_ClientThread()->Wait_JobDone();
#endif // _OVERDRIVE_LOAD_EFFECTGROUP_
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
		switch (m_eMyLevel)
		{
		case Client::LEVEL_LOGO:
			GAMEINSTANCE->PlayBGM("BGM_LOGO.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_BGM));
			break;
		case Client::LEVEL_LOBBY:
			break;
		case Client::LEVEL_GAMEPLAY:
		{
			// BGM
			GAMEINSTANCE->PlayBGM("BGM_STAGE_1.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_BGM));

			// 환경음
			GAMEINSTANCE->PlaySound2D("Env_HorrorBG_01.ogg", 0.5f, false);
			GAMEINSTANCE->PlaySound2D("Env_Wind_Howling_01.ogg", 0.5f, true);
			GAMEINSTANCE->PlaySound2D("Env_DarkForest_01.ogg", 0.5f, true);
			GAMEINSTANCE->PlaySound2D("Env_RainLight_01.ogg", 0.5f, true);
		}
			break;
		case Client::LEVEL_STAGE2:
		{
			// BGM
			GAMEINSTANCE->PlayBGM("BGM_STAGE_2.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_BGM));

			// 환경음
			GAMEINSTANCE->PlaySound2D("Env_BloodWater_01.ogg", 0.5f, true);
			GAMEINSTANCE->PlaySound2D("Env_Crystal_02.ogg", 0.5f, true);
			GAMEINSTANCE->PlaySound2D("Env_Wind_Abyss_01.ogg", 0.5f, true);
			GAMEINSTANCE->PlaySound2D("Env_WaterDrop_02.ogg", 0.75f, true);
			GAMEINSTANCE->PlaySound2D("Env_WaterDrop_03.ogg", 0.75f, true);
			GAMEINSTANCE->PlaySound2D("Env_WaterDrop_04.ogg", 0.75f, true);
		}
			break;
		case Client::LEVEL_STAGE3:
		{
			// BGM
			GAMEINSTANCE->PlayBGM("BGM_STAGE_3.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_BGM));

			// 환경음
			GAMEINSTANCE->PlaySound2D("Env_BloodWater_01.ogg", 0.5f, true);
			GAMEINSTANCE->PlaySound2D("Env_Crystal_02.ogg", 0.5f, true);
			GAMEINSTANCE->PlaySound2D("Env_Wind_Abyss_01.ogg", 0.5f, true);
			GAMEINSTANCE->PlaySound2D("Env_WaterDrop_02.ogg", 0.75f, true);
			GAMEINSTANCE->PlaySound2D("Env_WaterDrop_03.ogg", 0.75f, true);
			GAMEINSTANCE->PlaySound2D("Env_WaterDrop_04.ogg", 0.75f, true);
		}
			break;
		case Client::LEVEL_TEST:
			GAMEINSTANCE->PlayBGM("BGM_STAGE_TEST.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_BGM));
			break;
		case Client::LEVEL_END:
			break;
		default:
			break;
		}
		m_bLading = true;
		Call_StageLanding();
		SaveLevel();
	}
}

void CClientLevel::SetUp_UI()
{
	weak_ptr<CGameManager>	pGameManager = GET_SINGLE(CGameManager);
	weak_ptr<CUIManager>	pUIManager = GET_SINGLE(CUIManager);

	if (m_eMyLevel == LEVEL_LOADING || m_eMyLevel == LEVEL_LOGO || m_eMyLevel == LEVEL_EDIT)
		return;

	if (pUIManager.lock()->Get_Completed_SetUpUI())
	{
		m_pPauseMenu = GAMEINSTANCE->Get_GameObjects< CUI_PauseMenu>(LEVEL_STATIC).front();
		m_pEvolveMenu = GAMEINSTANCE->Get_GameObjects< CUI_EvolveMenu>(LEVEL_STATIC).front();
		//이미 셋업된 UI면 리턴한다.
		return;
	}

	GAMEINSTANCE->Add_SingleGameObject<CUI_RadialBlurMask>(LEVEL_STATIC);
	GAMEINSTANCE->Add_SingleGameObject<CUI_FadeMask>(LEVEL_STATIC);

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

	pUIManager.lock()->Set_Complete_SetUpUI();
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
		if (m_pEvolveMenu.lock()->Get_Enable() == true)
			return;

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

void CClientLevel::SaveLevel()
{
	if (m_eMyLevel == LEVEL_LOADING || m_eMyLevel == LEVEL_LOGO || m_eMyLevel == LEVEL_EDIT)
		return;


	json	                LevelJson;
	string                  szClientSavePath = "../Bin/SaveLevelData/SaveData.json";

	LevelJson["Level"] = m_eMyLevel;

	CJson_Utility::Save_Json(szClientSavePath.c_str(), LevelJson);
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
	//GAMEINSTANCE->StopSound(0);
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
