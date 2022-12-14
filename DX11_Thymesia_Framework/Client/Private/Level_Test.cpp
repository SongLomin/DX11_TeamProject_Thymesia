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

#ifdef _JOJO_EFFECTS_
	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_TEST);
#endif // _JOJO_EFFECTS_

#ifdef	_STAGE_1_MONSTER_
	Load_FromJson(m_szDefaultJsonPath + "Stage1_sub.json", LEVEL::LEVEL_TEST);
#endif

#ifdef _TEST_STATIC_PROPS_
	//Load_FromJson(m_szDefaultJsonPath + "Test_Level.json", LEVEL::LEVEL_TEST);
#else // _TEST_STATIC_PROPS_
	Load_FromJson(m_szDefaultJsonPath + "Stage1.json", LEVEL::LEVEL_TEST);
#endif // _TEST_STATIC_PROPS_
	//Load_FromJson(m_szDefaultJsonPath + "Stage1_sub.json", LEVEL::LEVEL_TEST);
	Load_FromJson(m_szDefaultJsonPath + "Test_Level.json", LEVEL::LEVEL_TEST);
	//Load_FromJson(m_szDefaultJsonPath + "Stage1_Song.json", LEVEL::LEVEL_TEST);

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

#ifdef _TEST_DYNAMIC_PROPS_
	GAMEINSTANCE->Add_GameObject<CDynamic_Prop>(LEVEL_TEST).lock()->Get_Transform()->Add_Position({ 5.f, 2.f, 5.f });
	GAMEINSTANCE->Add_GameObject<CDynamic_Prop>(LEVEL_TEST).lock()->Get_Transform()->Add_Position({ 15.f, 2.f, 15.f });
#endif
	GAMEINSTANCE->Set_FogDesc(_float4(1.f, 1.f, 1.f, 0.f), 9999.f);
	GAMEINSTANCE->Set_LiftGammaGain(_float4(1.f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f));

	GAMEINSTANCE->Add_GameObject<CLight_Prop>(LEVEL_TEST);
	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_TEST);
	GAMEINSTANCE->Set_ShadowLight({ -15.f, 30.f, -15.f }, { 0.f, 0.f, 0.f });
	GAMEINSTANCE->Set_FogDesc(_float4(1.f, 0.95f, 0.95f, 0.f), 9999.f);
	GAMEINSTANCE->Set_LiftGammaGain(_float4(1.f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f));
	SetUp_UI();
	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

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

#ifndef _LOAD_CAPTURED_RESOURCE_
	if (KEY_INPUT(KEY::HOME, KEY_STATE::TAP))
		GAMEINSTANCE->Write_JsonUsingResource("../Bin/LevelData/CapturedResource/TestLevel.json");
#endif // _LOAD_CAPTURED_RESOURCE_
}


HRESULT CLevel_Test::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Thymesia : TEST"));
	return S_OK;
}



shared_ptr<CLevel_Test> CLevel_Test::Create()
{
	shared_ptr<CLevel_Test>		pInstance = make_shared<CLevel_Test>();
	pInstance->m_eMyLevel = LEVEL_TEST;
	pInstance->Initialize();
	return pInstance;
}

void CLevel_Test::ExitLevel(LEVEL eLevel)
{
	switch (eLevel)
	{
		case  LEVEL::LEVEL_STAGE2:
		case  LEVEL::LEVEL_STAGE3:
		{
			FaderDesc tFaderDesc;
			tFaderDesc.eFaderType	= FADER_TYPE::FADER_OUT;
			tFaderDesc.eLinearType	= LINEAR_TYPE::LNIEAR;
			tFaderDesc.fFadeMaxTime = 1.f;
			tFaderDesc.fDelayTime	= 0.5f;
			tFaderDesc.vFadeColor	= _float4(0.f, 0.f, 0.f, 1.f);

			m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();
			m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
			m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_FadeOutToLevelChange, this);
		}
		break;
	}

}
void CLevel_Test::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);
}

void CLevel_Test::Free()
{

}



