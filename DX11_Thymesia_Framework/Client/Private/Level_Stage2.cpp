#include "stdafx.h"
#include "Level_Stage2.h"
#include "Player.h"
#include "GameInstance.h"
#include "Client_GameObjects.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "Fader.h"
#include "UI_EvolveMenu.h"
#include "UI_PauseMenu.h"
#include "PhysXCharacterController.h"

CLevel_Stage2::CLevel_Stage2()
{

}

CLevel_Stage2::~CLevel_Stage2()
{
	Free();
}

HRESULT CLevel_Stage2::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	Load_FromJson(m_szDefaultJsonPath + "Stage2-2.json", LEVEL::LEVEL_STAGE2);

	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_STAGE2);

	CCamera::CAMERADESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye    = _float4(0.0f, 2.5f, -2.5f, 1.f);
	CameraDesc.vAt     = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy   = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear   = 0.01f;
	CameraDesc.fFar    = 60.f;

	weak_ptr<CCamera_Target> TargetCamera = GAMEINSTANCE->Add_GameObject<CCamera_Target>(LEVEL::LEVEL_STAGE2, &CameraDesc);
	GET_SINGLE(CGameManager)->Set_TargetCamera(TargetCamera);

	GAMEINSTANCE->Set_CameraFar(CameraDesc.fFar);


	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_STAGE2);
	GAMEINSTANCE->Set_ShadowLight({ -15.f, 30.f, -15.f }, { 0.f, 0.f, 0.f });


	GET_SINGLE(CGameManager)->Set_PreLevel(LEVEL::LEVEL_STAGE2);
	GAMEINSTANCE->Set_GodRay_Enable(false);
	m_pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	SetUp_UI();

	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL::LEVEL_STATIC).front();
	
	return S_OK;
}


void CLevel_Stage2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	Tick_Key_InputEvent();
	if (KEY_INPUT(KEY::HOME, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Write_JsonUsingResource("../Bin/LevelData/CapturedResource/Stage2.json");
	}

	_vector vPosition = m_pPlayer.lock()->Get_WorldPosition();

	GAMEINSTANCE->Set_DynamicShadowLight(
		{ -15.f + XMVectorGetX(vPosition)
		, 30.f + XMVectorGetY(vPosition)
		, -15.f + XMVectorGetZ(vPosition) }
		, { XMVectorGetX(vPosition)
			, XMVectorGetY(vPosition)
			, XMVectorGetZ(vPosition) });
	
}

HRESULT CLevel_Stage2::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(__super::Render(pDeviceContext)))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("Thymesia : STAGE 2"));

	return S_OK;
}

void CLevel_Stage2::ExitLevel(LEVEL eLevel)
{
	__super::ExitLevel(eLevel);

	switch (eLevel)
	{
		case  LEVEL::LEVEL_TEST:
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

shared_ptr<CLevel_Stage2> CLevel_Stage2::Create()
{
	shared_ptr<CLevel_Stage2>		pInstance = make_shared<CLevel_Stage2>();
	pInstance->m_eMyLevel = LEVEL_STAGE2;
	pInstance->Initialize();

	return pInstance;
}

void CLevel_Stage2::Free()
{

}


