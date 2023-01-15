#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "Fader.h"
#include "GameManager.h"
#include "Client_GameObjects.h"
#include "Player_HPBar.h"
#include "Player_MPBar.h"
#include "UI_Logo.h"


CLevel_Logo::CLevel_Logo()
{

}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	GAMEINSTANCE->Create_Scene(CPhysX_Manager::SCENE_CURRENT);

	CCamera::CAMERADESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye    = _float4(0.0f, 1.2f, 1.5f, 1.f);
	CameraDesc.vAt     = _float4(0.f, 1.2f, 0.f, 1.f);
	CameraDesc.fFovy   = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear   = 0.01f;
	CameraDesc.fFar    = 300.f;
	
	GAMEINSTANCE->Set_CameraFar(CameraDesc.fFar);

	m_eNextLevel = LEVEL_LOBBY;

	Load_FromJson(m_szDefaultJsonPath + "Logo.json", LEVEL_LOGO);

	GAMEINSTANCE->Add_GameObject<CUI_Logo>(LEVEL_LOGO);
	//GAMEINSTANCE->Add_GameObject<CTargetCurve>(LEVEL_LOGO);

	GAMEINSTANCE->Add_SingleGameObject<CTargetCurve>(LEVEL_STATIC);

	GAMEINSTANCE->Load_Model("P_DropTombstone01", "../Bin/Resources/Meshes/Map_Lv1_Circus/Binary/P_DropTombstone01.bin", MODEL_TYPE::NONANIM, XMMatrixIdentity());
	GAMEINSTANCE->Add_SingleGameObject<CInteraction_DeadSpot>(LEVEL::LEVEL_STATIC);

	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	if (KEY_INPUT(KEY::F2, KEY_STATE::TAP))
	{
		GAMEINSTANCE->StopAll();
		if (FAILED(GAMEINSTANCE->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_EDIT))))
			return;
	}

	if (KEY_INPUT(KEY::HOME, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Write_JsonUsingResource("../Bin/LevelData/CapturedResource/Logo.json");
	}
}

HRESULT CLevel_Logo::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(__super::Render(pDeviceContext)))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Thymesia : LOGO"));

	return S_OK;
}

void CLevel_Logo::ExitLevel(LEVEL eLevel)
{
	__super::ExitLevel(eLevel);

	m_eNextLevel = eLevel;

//	if (eLevel == LEVEL::LEVEL_GAMEPLAY)
//	{
		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 1.f;
		tFaderDesc.fDelayTime = 0.5f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();
		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_FadeOutToLevelChange, this);
	/*
	}
	else if (eLevel == LEVEL::LEVEL_GAMEPLAY)
	{
		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 0.1f;
		tFaderDesc.fDelayTime = 0.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();
		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_FadeOutToLevelChange, this);
	}
	else if (eLevel == LEVEL::LEVEL_TEST)
	{
		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 0.1f;
		tFaderDesc.fDelayTime = 0.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();
		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_FadeOutToLevelChange, this);
	}
	*/
}



shared_ptr<CLevel_Logo> CLevel_Logo::Create()
{
	shared_ptr<CLevel_Logo>		pInstance = make_shared<CLevel_Logo>();
	pInstance->m_eMyLevel = LEVEL_LOGO;
	pInstance->Initialize();

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}

void CLevel_Logo::SetUp_UI()
{
	return;
}

