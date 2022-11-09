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

	ShowCursor(false);

	
	Load_FromJson(m_szDefaultJsonPath + "Stage2.json", LEVEL::LEVEL_STAGE2);

	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_STAGE2);

	GAMEINSTANCE->Add_GameObject<CEditNaviMesh>(LEVEL_STAGE2).lock()->Load_DebugNaviMesh("Stage2_Navi");

	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

	FaderDesc tFaderDesc;
	tFaderDesc.eFaderType = FADER_TYPE::FADER_IN;
	tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
	tFaderDesc.fFadeMaxTime = 3.f;
	tFaderDesc.fDelayTime = 0.5f;
	tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.f);

	m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);

	GET_SINGLE(CGameManager)->Set_GameState(GAME_STATE::PEACE);



	GAMEINSTANCE->Set_ShadowLight({ -15.f, 75.f, -200.f }, { 0.f, 0.f, 0.f });

	GAMEINSTANCE->PlayBGM(TEXT("Stage2.wav"), 0.3f);
	GAMEINSTANCE->PlaySoundW(TEXT("Rain.wav"), (_uint)1, 0.4f);


	return S_OK;
}

void CLevel_Stage2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	if (m_bChangeNextLevel)
	{
		m_bChangeNextLevel = false;

		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::POW;
		tFaderDesc.fFadeMaxTime = 1.f;
		tFaderDesc.fDelayTime = 50.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
	}

	/*if (KEY_INPUT(KEY::DELETEKEY, KEY_STATE::TAP))
	{
		m_eNextLevel = LEVEL_STAGE3;

		CFader::FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 1.f;
		tFaderDesc.fDelayTime = 0.5f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_FadeOutToLevelChange, this);

	}*/
}

HRESULT CLevel_Stage2::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임프렐이레벨임. "));

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

