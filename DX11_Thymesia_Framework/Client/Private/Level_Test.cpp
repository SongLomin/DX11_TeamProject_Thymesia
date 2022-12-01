#include "stdafx.h"
#include "Level_Test.h"
#include "GameInstance.h"
#include "Client_GameObjects.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "Fader.h"
#include <UI_Landing.h>
#include "Static_Instancing_Prop.h"
#include "HUD_PlagueWeapon.h"
#include "UI_PauseMenu.h"
#include "MonsterHPBar_Base.h"
#include "MonsterHPBar_Elite.h"
#include "MonsterHPBar_Boss.h"
#include "UI_EvolveMenu.h"
#include "Client_Presets.h"


CLevel_Test::CLevel_Test()
//: CLevel(pDevice, pContext) ID3D11Device* pDevice, ID3D11DeviceContext* pContext
{

}

CLevel_Test::~CLevel_Test()
{
	Free();
}

HRESULT CLevel_Test::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	ShowCursor(false);
	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_TEST);

#ifdef _TEST_STATIC_PROPS_
	Load_FromJson(m_szDefaultJsonPath + "Stage3.json", LEVEL::LEVEL_TEST);
	//Load_FromJson(m_szDefaultJsonPath + "Stage1_StaticProps.json", LEVEL::LEVEL_TEST);
#else
	Load_FromJson(m_szDefaultJsonPath + "Stage1.json", LEVEL::LEVEL_TEST);
#endif // _TEST_STATIC_PROPS_

	//Load_FromJson(m_szDefaultJsonPath + "Stage1_sub.json", LEVEL::LEVEL_Test);
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye = _float4(0.0f, 2.5f, -2.5f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	weak_ptr<CCamera_Target> TargetCamera = GAMEINSTANCE->Add_GameObject<CCamera_Target>(LEVEL::LEVEL_TEST, &CameraDesc);
	GET_SINGLE(CGameManager)->Set_TargetCamera(TargetCamera);

	weak_ptr<CCorvus> pCorvus = GAMEINSTANCE->Add_GameObject<CCorvus>(LEVEL_TEST);
	GET_SINGLE(CGameManager)->Set_CurrentPlayer(pCorvus);

	GAMEINSTANCE->Add_GameObject<CDynamic_Prop>(LEVEL_TEST).lock()->Get_Transform()->Add_Position({ 5.f, 0.f, 5.f });
	GAMEINSTANCE->Add_GameObject<CDynamic_Prop>(LEVEL_TEST).lock()->Get_Transform()->Add_Position({ 15.f, 0.f, 15.f });


	CMonster::STATE_LINK_MONSTER_DESC CCC;
#ifdef _VARG_
	ZeroMemory(&CCC, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	CCC.eMonType = MONSTERTYPE::VARG;
	CCC.eBossStartType = BOSSSTARTTYPE::BEGINSTART;
	CCC.vYame.x = 49.33f;
	CCC.vYame.y = 14.4f;
	CCC.vYame.z = 30.32f;
	GAMEINSTANCE->Add_GameObject<CVarg>(LEVEL_TEST, &CCC);
#endif // _VARG_


	//TODO 야매에요
#ifdef _STAGE_1_MONSTER_
	ZeroMemory(&CCC, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	CCC.eMonType = MONSTERTYPE::AXEMAN;
	CCC.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	CCC.vYame.x = 13.f;
	CCC.vYame.y = 4.6f;
	CCC.vYame.z = 36.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_TEST, &CCC);

	ZeroMemory(&CCC, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	CCC.eMonType = MONSTERTYPE::KNIFEWOMAN;
	CCC.eNorMonIdleType = NORMONSTERIDLETYPE::SITIDLE;
	CCC.vYame.x = 23.f;
	CCC.vYame.y = 4.6f;
	CCC.vYame.z = 43.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_TEST, &CCC);

	ZeroMemory(&CCC, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	CCC.eMonType = MONSTERTYPE::GARDENER;
	CCC.eNorMonIdleType = NORMONSTERIDLETYPE::FIDGETIDLE;
	CCC.vYame.x = 2.5f;
	CCC.vYame.y = 4.9f;
	CCC.vYame.z = 46.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_TEST, &CCC);

	ZeroMemory(&CCC, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	CCC.eMonType = MONSTERTYPE::SHIELDAXEMAN;
	CCC.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	CCC.vYame.x = 2.5f;
	CCC.vYame.y = 4.9f;
	CCC.vYame.z = 46.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_TEST, &CCC);
#endif // _STAGE_1_MONSTER_

	//야매에요

	GAMEINSTANCE->Add_GameObject<CLight_Prop>(LEVEL_TEST);

	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_TEST);

	GAMEINSTANCE->Set_ShadowLight({ -15.f, 30.f, -15.f }, { 0.f, 0.f, 0.f });


	SetUp_UI();
	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

	Preset::AddGameObject::TalentSetting();

	return S_OK;
}

void CLevel_Test::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

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
			m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CLevel_Test::Call_Enable_PauseMenu, this);
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

		m_bFadeTrigger = true;
	}
	/*if (m_bChangeNextLevel)
	{
		m_bChangeNextLevel = false;

		m_eNextLevel = LEVEL_STAGE2;

		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::POW;
		tFaderDesc.fFadeMaxTime = 1.f;
		tFaderDesc.fDelayTime = 0.5f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_FadeOutToLevelChange, this);

	}*/

	if (KEY_INPUT(KEY::ALT, KEY_STATE::TAP))
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
			m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CLevel_Test::Call_Enable_EvolveMenu, this);
		}
	}

}


HRESULT CLevel_Test::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임프렐이레벨임. "));

	return S_OK;
}



shared_ptr<CLevel_Test> CLevel_Test::Create()
{
	shared_ptr<CLevel_Test>		pInstance = make_shared<CLevel_Test>();
	pInstance->m_eMyLevel = LEVEL_TEST;
	pInstance->Initialize();
	return pInstance;
}

void CLevel_Test::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);
}

void CLevel_Test::Free()
{

}



