#include "stdafx.h"
#include "Level_Stage3.h"

#include "GameInstance.h"
#include "Client_GameObjects.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "Fader.h"
#include "UI_EvolveMenu.h"
#include "UI_PauseMenu.h"
#include "PhysXCharacterController.h"


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

	GET_SINGLE(CGameInstance)->Update_VolumeScale(5.f);
	
	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_STAGE3);

	CCamera::CAMERADESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye    = _float4(0.0f, 2.5f, -2.5f, 1.f);
	CameraDesc.vAt     = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy   = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear   = 0.01f;
	CameraDesc.fFar    = 50.f;

	weak_ptr<CCamera_Target> TargetCamera = GAMEINSTANCE->Add_GameObject<CCamera_Target>(LEVEL::LEVEL_STAGE3, &CameraDesc);
	GET_SINGLE(CGameManager)->Set_TargetCamera(TargetCamera);

	GAMEINSTANCE->Set_CameraFar(CameraDesc.fFar);

	Load_FromJson(m_szDefaultJsonPath + "Stage_Lv3-1.json", LEVEL::LEVEL_STAGE3);

	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL::LEVEL_STAGE3);
	GAMEINSTANCE->Set_ShadowLight({ -50.67f, 50.f, 40.f, 1.f }, { 3.45f, 0.f, 40.f });

	m_pFadeMask   = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL::LEVEL_STATIC).front();
	GAMEINSTANCE->Set_GodRay_Enable(true);
	GAMEINSTANCE->Set_GodRayDesc(_float4(0.7f, 0.7f, 0.7f, 1.f), _float4(3.01f, 25.f, 40.2f, 1.f));

	m_pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	GET_SINGLE(CGameManager)->Set_PreLevel(LEVEL::LEVEL_STAGE3);

	SetUp_UI();

	return S_OK;
}

void CLevel_Stage3::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	Tick_Key_InputEvent();
	if (KEY_INPUT(KEY::HOME, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Write_JsonUsingResource("../Bin/LevelData/CapturedResource/Stage3.json");
	}

	_vector vPosition = m_pPlayer.lock()->Get_WorldPosition();

	GAMEINSTANCE->Set_DynamicShadowLight(
		{ -50.67f + XMVectorGetX(vPosition)
		, 50.f + XMVectorGetY(vPosition)
		, 40.f + XMVectorGetZ(vPosition) }
		, { 3.45f+XMVectorGetX(vPosition)
			, XMVectorGetY(vPosition)
			,40.f+ XMVectorGetZ(vPosition) });
}

HRESULT CLevel_Stage3::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(__super::Render(pDeviceContext)))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Thymesia : STAGE 3"));

	return S_OK;
}

shared_ptr<CLevel_Stage3> CLevel_Stage3::Create()
{
	shared_ptr<CLevel_Stage3> pInstance = make_shared<CLevel_Stage3>();
	pInstance->m_eMyLevel = LEVEL::LEVEL_STAGE3;
	pInstance->Initialize();

	return pInstance;
}

void CLevel_Stage3::Free()
{

}

