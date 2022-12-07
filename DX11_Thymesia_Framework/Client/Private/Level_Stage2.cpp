#include "stdafx.h"
#include "Level_Stage2.h"
#include "GameInstance.h"
#include "Client_GameObjects.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "Fader.h"


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

#ifndef  _ONLY_UI_

	Load_FromJson(m_szDefaultJsonPath + "Stage2-2.json", LEVEL::LEVEL_STAGE2);

	CCamera::CAMERADESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye = _float4(0.0f, 2.5f, -2.5f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	weak_ptr<CCamera_Target> TargetCamera = GAMEINSTANCE->Add_GameObject<CCamera_Target>(LEVEL::LEVEL_GAMEPLAY, &CameraDesc);
	GET_SINGLE(CGameManager)->Set_TargetCamera(TargetCamera);

	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_GAMEPLAY);
	GAMEINSTANCE->Set_ShadowLight({ -15.f, 30.f, -15.f }, { 0.f, 0.f, 0.f });

#endif // ! _ONLY_UI_

	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

	return S_OK;
}

void CLevel_Stage2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

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

	if (KEY_INPUT(KEY::HOME, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Write_JsonUsingResource("../Bin/LevelData/CapturedResource/Stage2.json");
	}
}

HRESULT CLevel_Stage2::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("Thymesia : STAGE 2"));

	return S_OK;
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

