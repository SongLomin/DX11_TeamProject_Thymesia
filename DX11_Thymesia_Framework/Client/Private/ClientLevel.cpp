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


GAMECLASS_C(CClientLevel)

HRESULT CClientLevel::Initialize()
{
	__super::Initialize();

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
	fs::directory_iterator itr(In_FolderPath);
	 
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

}

void CClientLevel::SetUp_UI()
{
	weak_ptr<CGameManager>	pGameManager = GET_SINGLE(CGameManager);

	pGameManager.lock()->SetCursor(GAMEINSTANCE->Add_GameObject<CUI_Cursor>(LEVEL_STATIC));

	GAMEINSTANCE->Add_GameObject<CUI_Landing>(LEVEL_STATIC);//여기서
	m_pPauseMenu = GAMEINSTANCE->Add_GameObject<CUI_PauseMenu>(LEVEL_STATIC);

	m_pEvolveMenu = GAMEINSTANCE->Add_GameObject<CUI_EvolveMenu>(LEVEL_STATIC);
	GAMEINSTANCE->Add_GameObject<CUI_EvolveMenu_Level>(LEVEL_STATIC);

	Preset::AddGameObject::TalentSetting();

	pGameManager.lock()->Register_Layer(OBJECT_LAYER::PLAYERHUD, GAMEINSTANCE->Add_GameObject<CPlayer_HPBar>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::PLAYERHUD, GAMEINSTANCE->Add_GameObject<CPlayer_MPBar>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::PLAYERHUD, GAMEINSTANCE->Add_GameObject<CHUD_Player_Memory>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::PLAYERHUD, GAMEINSTANCE->Add_GameObject<CHUD_PlagueWeapon>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::PLAYERHUD, GAMEINSTANCE->Add_GameObject<CPlayer_PotionUI>(LEVEL_STATIC));                                                                                                  pGameManager.lock()->Register_Layer(OBJECT_LAYER::PLAYERHUD, GAMEINSTANCE->Add_GameObject<CPlayer_FeatherUI>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::EVENT_UI, GAMEINSTANCE->Add_GameObject<CUI_ScriptQueue>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::INTERACTIONUI, GAMEINSTANCE->Add_GameObject<CUI_Interaction>(LEVEL_STATIC));

	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CUI_MonsterFocus>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::EVENT_UI, GAMEINSTANCE->Add_GameObject<CUI_AppearEventVarg>(LEVEL_STATIC));

	GAMEINSTANCE->Add_GameObject<CUI_BloodOverlay>(LEVEL_STATIC);

	GET_SINGLE(CGameManager)->CreatePopupQueue();


	//GET_SINGLE(CGameManager)->DisableCursor();

#ifdef _ONLY_UI_

	GAMEINSTANCE->Add_GameObject<CTestUI>(LEVEL_STATIC);

#endif




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
#ifdef _ONLY_UI_
	if (KEY_INPUT(KEY::T, KEY_STATE::TAP))
	{
		if (m_pEvolveMenu.lock()->Get_Enable() == false)
		{
			FaderDesc tFaderDesc;
			tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
			tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
			tFaderDesc.fFadeMaxTime = 0.2f;
			tFaderDesc.fDelayTime = 0.f;
			tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

			m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
			m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_Enable_EvolveMenu, this);
		}
	}
	if (KEY_INPUT(KEY::V, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Add_GameObject<CUI_DamageFont>(LEVEL_STATIC).lock()->SetUp_DamageFont
		(
			557,
			_float2(g_iWinCX >> 1, g_iWinCY >> 1),
			Client::ATTACK_OPTION::NORMAL
		);
	}
	if (KEY_INPUT(KEY::NUM2, KEY_STATE::TAP))
	{
		if (m_pPauseMenu.lock()->Get_Enable() == false)
		{
			ExitLevel(LEVEL::LEVEL_STAGE2);
		}
	}
#endif // _ONLY_UI_
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
