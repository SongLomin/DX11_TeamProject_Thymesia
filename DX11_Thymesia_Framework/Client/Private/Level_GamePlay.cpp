#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
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
#include "MonsterParryingBar.h"
#include "UI_Containers.h"
#include "UI_EvolveMenu.h"
#include "UI_EvolveMenu_Level.h"
#include "UI_Script.h"
#include "UI_ScriptQueue.h"
#include "Player.h"
#include "UI_DamageFont.h"

CLevel_GamePlay::CLevel_GamePlay()
{
}

CLevel_GamePlay::~CLevel_GamePlay() { Free(); }

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Setting Camera..."));

	CCamera::CAMERADESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye = _float4(0.0f, 2.5f, -2.5f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.01f;
	CameraDesc.fFar = 20.f;

	GAMEINSTANCE->Set_CameraFar(CameraDesc.fFar);

	weak_ptr<CCamera_Target> TargetCamera = GAMEINSTANCE->Add_GameObject<CCamera_Target>(LEVEL::LEVEL_GAMEPLAY, &CameraDesc);
	GET_SINGLE(CGameManager)->Set_TargetCamera(TargetCamera);

	SetWindowText(g_hWnd, TEXT("Loading All Effect Group..."));
	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_GAMEPLAY);

	SetWindowText(g_hWnd, TEXT("Loading From Json : [ Stage2.json ]"));
	Load_FromJson(m_szDefaultJsonPath + "Stage2.json", LEVEL::LEVEL_GAMEPLAY);

	SetWindowText(g_hWnd, TEXT("Adding Light Prop & Skybox..."));
	GAMEINSTANCE->Add_GameObject<CLight_Prop>(LEVEL_GAMEPLAY);
	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_GAMEPLAY);
	GAMEINSTANCE->Set_ShadowLight({ -15.f, 30.f, -15.f }, { 0.f, 0.f, 0.f });

	GAMEINSTANCE->Set_GodRay_Enable(false);

	SetWindowText(g_hWnd, TEXT("Setting up UI..."));
	SetUp_UI();
	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

	SetWindowText(g_hWnd, TEXT("Setting Player..."));
	m_pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	GET_SINGLE(CGameManager)->Set_PreLevel(LEVEL::LEVEL_GAMEPLAY);

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Tick_Key_InputEvent();

#ifndef _LOAD_CAPTURED_RESOURCE_
	if (KEY_INPUT(KEY::HOME, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Write_JsonUsingResource("../Bin/LevelData/CapturedResource/GamePlay.json");
	}
#endif // _LOAD_CAPTURED_RESOURCE_
	_vector vPosition = m_pPlayer.lock()->Get_WorldPosition();

	GAMEINSTANCE->Set_DynamicShadowLight(
		{ -15.f + XMVectorGetX(vPosition)
		, 30.f + XMVectorGetY(vPosition)
		, -15.f + XMVectorGetZ(vPosition) }
	, { XMVectorGetX(vPosition)
		, XMVectorGetY(vPosition)
		, XMVectorGetZ(vPosition) });
}

HRESULT CLevel_GamePlay::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(__super::Render(pDeviceContext)))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("Thymesia : GAMEPLAY"));
	return S_OK;
}

void CLevel_GamePlay::ExitLevel(LEVEL eLevel)
{
	__super::ExitLevel(eLevel);

	if (eLevel == LEVEL::LEVEL_STAGE2)
	{
		CStatus_Player::PLAYERDESC tPlayerDesc = GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status().lock()->Get_Desc();
		
		m_eNextLevel = eLevel;

		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 1.f;
		tFaderDesc.fDelayTime = 0.5f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();
		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_FadeOutToLevelChange, this);
	}
}


shared_ptr<CLevel_GamePlay> CLevel_GamePlay::Create()
{
	shared_ptr<CLevel_GamePlay>		pInstance = make_shared<CLevel_GamePlay>();
	pInstance->m_eMyLevel = LEVEL_GAMEPLAY;
	pInstance->Initialize();
	return pInstance;
}

void CLevel_GamePlay::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);
}

void CLevel_GamePlay::Free()
{

}

