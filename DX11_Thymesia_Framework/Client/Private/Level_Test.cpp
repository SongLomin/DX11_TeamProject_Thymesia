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
#include "Model.h"
#include "UI_EffectGroup_SkillOpen.h"

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

	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_TEST);

	// Load_FromJson(m_szDefaultJsonPath + "Test_Level.json", LEVEL::LEVEL_TEST);
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

	GAMEINSTANCE->Add_GameObject<CLight_Prop>(LEVEL_TEST);
	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_TEST);

	GAMEINSTANCE->Set_ShadowLight({ 50.f, -50.f, -50.f }, { 0.f, 0.f, 0.f });

	GAMEINSTANCE->Set_FogDesc(_float4(1.f, 0.95f, 0.95f, 0.f), 9999.f);
	GAMEINSTANCE->Set_LiftGammaGain(_float4(1.f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f));

	GAMEINSTANCE->Set_GodRayDesc(_float4(0.7f, 0.7f, 0.7f, 1.f), _float4(3.01f, 25.f, 40.2f, 1.f));
	GAMEINSTANCE->Set_GodRayScale(1.f);

	m_pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

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

	if (KEY_INPUT(KEY::NUMPAD1, KEY_STATE::TAP))
	{
		weak_ptr<CUI_EffectGroup_SkillOpen> pEffectGroup = GET_SINGLE(CGameManager)->
			GetGameObject_SafetyUseMemoryPool< CUI_EffectGroup_SkillOpen>(LEVEL_STATIC);

		pEffectGroup.lock()->Play(SKILL_NAME::SKILL_AXE);
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

	_vector vPosition = m_pPlayer.lock()->Get_WorldPosition();

	GAMEINSTANCE->Set_DynamicShadowLight(
		{ -15.f + XMVectorGetX(vPosition)
		, 30.f + XMVectorGetY(vPosition)
		, -15.f + XMVectorGetZ(vPosition) }
		, { XMVectorGetX(vPosition)
			, XMVectorGetY(vPosition)
			, XMVectorGetZ(vPosition) });

#ifndef _LOAD_CAPTURED_RESOURCE_
	if (KEY_INPUT(KEY::HOME, KEY_STATE::TAP))
		GAMEINSTANCE->Write_JsonUsingResource("../Bin/LevelData/CapturedResource/TestLevel.json");
#endif // _LOAD_CAPTURED_RESOURCE_
}


HRESULT CLevel_Test::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(__super::Render(pDeviceContext)))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Thymesia : TEST"));
	return S_OK;
}



shared_ptr<CLevel_Test> CLevel_Test::Create()
{
	shared_ptr<CLevel_Test> pInstance = make_shared<CLevel_Test>();
	pInstance->m_eMyLevel = LEVEL::LEVEL_TEST;
	pInstance->Initialize();

	return pInstance;
}

void CLevel_Test::ExitLevel(LEVEL eLevel)
{
	switch (eLevel)
	{
		case  LEVEL::LEVEL_GAMEPLAY:
		case  LEVEL::LEVEL_STAGE2:
		case  LEVEL::LEVEL_STAGE3:
		case  LEVEL::LEVEL_HOME:
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
void CLevel_Test::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);
}

void CLevel_Test::Free()
{

}



