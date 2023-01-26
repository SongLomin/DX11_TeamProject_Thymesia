#include "stdafx.h"
#include "Level_Home.h"

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
#include "Model.h"


CLevel_Home::CLevel_Home()
{

}

CLevel_Home::~CLevel_Home()
{
	Free();
}

HRESULT CLevel_Home::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_TEST);

	Load_FromJson(m_szDefaultJsonPath + "Stage_Home.json", LEVEL::LEVEL_TEST);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye     = _float4(0.0f, 2.5f, -2.5f, 1.f);
	CameraDesc.vAt      = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy    = XMConvertToRadians(65.0f);
	CameraDesc.fAspect  = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear    = 0.01f;
	CameraDesc.fFar     = 300.f;

	weak_ptr<CCamera_Target> TargetCamera = GAMEINSTANCE->Add_GameObject<CCamera_Target>(LEVEL::LEVEL_TEST, &CameraDesc);
	GET_SINGLE(CGameManager)->Set_TargetCamera(TargetCamera);

	GAMEINSTANCE->Set_CameraFar(CameraDesc.fFar);

	/*weak_ptr<CCorvus> pCorvus = GAMEINSTANCE->Add_GameObject<CCorvus>(LEVEL_TEST);
	GET_SINGLE(CGameManager)->Set_CurrentPlayer(pCorvus);*/


	GAMEINSTANCE->Add_GameObject<CLight_Prop>(LEVEL_TEST);
	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_TEST);

	GAMEINSTANCE->Set_ShadowLight({ -15.f, 30.f, -15.f }, { 0.f, 0.f, 0.f });

	GAMEINSTANCE->Set_FogDesc(_float4(1.f, 0.95f, 0.95f, 0.f), 9999.f);
	GAMEINSTANCE->Set_LiftGammaGain(_float4(1.f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f));

	SetUp_UI();
	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

	return S_OK;
}

void CLevel_Home::Tick(_float fTimeDelta)
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
			m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CLevel_Home::Call_Enable_PauseMenu, this);
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


HRESULT CLevel_Home::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(__super::Render(pDeviceContext)))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Thymesia : TEST"));
	return S_OK;
}

shared_ptr<CLevel_Home> CLevel_Home::Create()
{
	shared_ptr<CLevel_Home> pInstance = make_shared<CLevel_Home>();
	pInstance->m_eMyLevel = LEVEL::LEVEL_TEST;
	pInstance->Initialize();

	return pInstance;
}

void CLevel_Home::ExitLevel(LEVEL eLevel)
{
	switch (eLevel)
	{
		case  LEVEL::LEVEL_GAMEPLAY:
		case  LEVEL::LEVEL_STAGE2:
		case  LEVEL::LEVEL_STAGE3:
		{
			m_eNextLevel = eLevel;

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
void CLevel_Home::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);
}

void CLevel_Home::Free()
{

}



