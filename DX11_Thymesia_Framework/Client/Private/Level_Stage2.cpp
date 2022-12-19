#include "stdafx.h"
#include "Level_Stage2.h"

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

	LIGHTDESC LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eActorType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.2f, 0.19f, 0.18f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vLightFlag = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.bEnable = true;
	LightDesc.fIntensity = 1.f;

	GAMEINSTANCE->Add_Light(LightDesc);
#ifndef  _ONLY_UI_
	
	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_STAGE2);

	CCamera::CAMERADESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye    = _float4(0.0f, 2.5f, -2.5f, 1.f);
	CameraDesc.vAt     = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy   = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear   = 0.2f;
	CameraDesc.fFar    = 300.f;

	weak_ptr<CCamera_Target> TargetCamera = GAMEINSTANCE->Add_GameObject<CCamera_Target>(LEVEL::LEVEL_GAMEPLAY, &CameraDesc);
	GET_SINGLE(CGameManager)->Set_TargetCamera(TargetCamera);

	Load_FromJson(m_szDefaultJsonPath + "Stage2-2.json", LEVEL::LEVEL_STAGE2);

	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_STAGE2);
	GAMEINSTANCE->Set_ShadowLight({ -15.f, 30.f, -15.f }, { 0.f, 0.f, 0.f });

#endif // ! _ONLY_UI_

	m_pFadeMask   = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL::LEVEL_STATIC).front();
    m_pEvolveMenu = GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu>(LEVEL::LEVEL_STATIC).front();
	m_pPauseMenu  = GAMEINSTANCE->Get_GameObjects<CUI_PauseMenu>(LEVEL::LEVEL_STATIC).front();

	GAMEINSTANCE->Set_FogDesc(_float4(1.f, 1.f, 1.f, 0.65f), 70.f);
	GAMEINSTANCE->Set_LiftGammaGain(_float4(1.f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f));

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
	
}

HRESULT CLevel_Stage2::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(__super::Render(pDeviceContext)))
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


