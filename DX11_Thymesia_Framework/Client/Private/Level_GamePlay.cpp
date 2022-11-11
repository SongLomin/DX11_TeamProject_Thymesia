#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Client_GameObjects.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "Fader.h"
#include <UI_Landing.h>


CLevel_GamePlay::CLevel_GamePlay()
	//: CLevel(pDevice, pContext) ID3D11Device* pDevice, ID3D11DeviceContext* pContext
{

}

CLevel_GamePlay::~CLevel_GamePlay()
{
	Free();
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	ShowCursor(false);

	

	//Load_FromJson(m_szDefaultJsonPath + "Stage1.json", LEVEL::LEVEL_GAMEPLAY);
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

	weak_ptr<CCorvus> pCorvus = GAMEINSTANCE->Add_GameObject<CCorvus>(LEVEL_GAMEPLAY);
	GET_SINGLE(CGameManager)->Set_CurrentPlayer(pCorvus);

	GAMEINSTANCE->Add_GameObject<CLight_Prop>(LEVEL_GAMEPLAY);

	GAMEINSTANCE->Add_GameObject<CTerrain>(LEVEL_GAMEPLAY);

	GET_SINGLE(CGameManager)->Register_Player_HPBar
	(GAMEINSTANCE->Add_GameObject<CPlayer_HPBar>(LEVEL_STATIC));

	GET_SINGLE(CGameManager)->Register_Player_MPBar
	(GAMEINSTANCE->Add_GameObject<CPlayer_MPBar>(LEVEL_STATIC));

	GET_SINGLE(CGameManager)->Register_Player_Memory
	(GAMEINSTANCE->Add_GameObject<CPlayer_Memory>(LEVEL_STATIC));

	weak_ptr<CPreViewAnimationModel> pPreviewModel = GAMEINSTANCE->Add_GameObject<CPreViewAnimationModel>(LEVEL_GAMEPLAY);
	pPreviewModel.lock()->Init_EditPreViewAnimationModel("Corvus");
	pPreviewModel.lock()->Change_AnimationFromIndex(3);

	pPreviewModel.lock()->Play_Animation(0.01f);
	pPreviewModel.lock()->Get_Component<CTransform>().lock()->Add_Position(XMVectorSet(10.f, 0.f, 10.f, 0.f));

	GET_SINGLE(CGameManager)->Register_Player_HUD_Potion(
		GAMEINSTANCE->Add_GameObject<CPlayer_PotionUI>(LEVEL_STATIC));

	GET_SINGLE(CGameManager)->Register_Player_HUD_Feather(
		GAMEINSTANCE->Add_GameObject<CPlayer_FeatherUI>(LEVEL_STATIC));

	GAMEINSTANCE->Add_GameObject<CUI_Landing>(LEVEL_STATIC);

	Load_FromJson(m_szDefaultJsonPath + "Stage1.json", LEVEL_GAMEPLAY);

	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();
	// GAMEINSTANCE->Add_GameObject<CGround>(LEVEL_GAMEPLAY);


	FaderDesc tFaderDesc;
	tFaderDesc.eFaderType = FADER_TYPE::FADER_IN;
	tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
	tFaderDesc.fFadeMaxTime = 3.f;
	tFaderDesc.fDelayTime = 0.5f;
	tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.f);

	m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);

	//GAMEINSTANCE->Add_GameObject<CDubian>(LEVEL::LEVEL_GAMEPLAY).lock()->Set_Enable(false);
	//GAMEINSTANCE->Add_GameObject<CBianka>(LEVEL::LEVEL_GAMEPLAY).lock()->Set_Enable(false);

	//GAMEINSTANCE->Add_GameObject<CStage1>(LEVEL::LEVEL_GAMEPLAY);
	

	//m_iMonsterCount = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::MONSTER).size();

	m_iMonsterCount = 0;

	GAMEINSTANCE->Set_ShadowLight({ -15.f, 30.f, -15.f }, { 0.f, 0.f, 0.f });


	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

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

}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임프렐이레벨임. "));

	return S_OK;
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

