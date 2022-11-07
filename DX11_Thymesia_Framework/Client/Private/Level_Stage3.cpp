#include "stdafx.h"
#include "Level_Stage3.h"
#include "GameInstance.h"
#include "Client_GameObjects.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "Fader.h"


CLevel_Stage3::CLevel_Stage3()
{

}

CLevel_Stage3::~CLevel_Stage3()
{
	Free();
}

HRESULT CLevel_Stage3::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	ShowCursor(false);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye = _float4(0.0f, 2.5f, -2.5f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	weak_ptr<CCamera_Target> TargetCamera = GAMEINSTANCE->Add_GameObject<CCamera_Target>(LEVEL::LEVEL_GAMEPLAY, &CameraDesc);

	GET_SINGLE(CGameManager)->Set_TargetCamera(TargetCamera);

	Load_FromJson(m_szDefaultJsonPath + "Stage3.json", LEVEL::LEVEL_STAGE3);

	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_GAMEPLAY);

	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();


	CFader::FaderDesc tFaderDesc;
	tFaderDesc.eFaderType = FADER_TYPE::FADER_IN;
	tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
	tFaderDesc.fFadeMaxTime = 3.f;
	tFaderDesc.fDelayTime = 0.5f;
	tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.f);

	m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);

	return S_OK;
}

void CLevel_Stage3::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	

}

HRESULT CLevel_Stage3::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임프렐이레벨임. "));

	return S_OK;
}

shared_ptr<CLevel_Stage3> CLevel_Stage3::Create()
{
	shared_ptr<CLevel_Stage3>		pInstance = make_shared<CLevel_Stage3>();
	pInstance->m_eMyLevel = LEVEL_GAMEPLAY;
	pInstance->Initialize();
	return pInstance;
}

void CLevel_Stage3::Free()
{

}

