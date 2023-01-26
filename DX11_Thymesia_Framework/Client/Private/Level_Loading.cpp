#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Level_Edit.h"
#include "Level_Test.h"
//#include "Level_Lobby.h"
#include "Level_Stage2.h"
#include "Level_Stage3.h"
#include "GameInstance.h"
#include "UI_Loading.h"
#include "FadeMask.h"
#include "GameManager.h"

CLevel_Loading::CLevel_Loading()
	//: CLevel(pDevice, pContext) ID3D11Device* pDevice, ID3D11DeviceContext* pContext
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	FaderDesc tFaderDesc;
	tFaderDesc.eFaderType = FADER_TYPE::FADER_IN;
	tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
	tFaderDesc.fFadeMaxTime = 1.f;
	tFaderDesc.fDelayTime = 0.5f;
	tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.f);

	weak_ptr<CGameObject> pFadeMaskFromGameObject = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::FADEMASK).front();
	weak_ptr<CFadeMask> pFadeMask = Weak_StaticCast<CFadeMask>(pFadeMaskFromGameObject);
	//pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);

	//일단 임시
	pFadeMask.lock()->Set_Enable(false);

	m_eNextLevel = eNextLevel;

	if (LEVEL::LEVEL_STATIC		!= m_eNextLevel &&
		LEVEL::LEVEL_LOADING	!= m_eNextLevel &&
		LEVEL::LEVEL_LOGO		!= m_eNextLevel &&
		LEVEL::LEVEL_LOBBY      != m_eNextLevel &&
		LEVEL::LEVEL_EDIT       != m_eNextLevel)
	{
		m_pUILoading = GAMEINSTANCE->Add_GameObject<CUI_Loading>(LEVEL_LOADING);
		m_pUILoading.lock()->SetUp_LoadingUI(m_eNextLevel);
	}

	m_pLoader = CLoader::Create(eNextLevel);
	if (nullptr == m_pLoader.get())
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY_INPUT(KEY::HOME, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Write_JsonUsingResource("../Bin/LevelData/CapturedResource/Loading.json");
	}

	if (true == m_pLoader->is_Finished())
	{
		if (LEVEL::LEVEL_STATIC		!= m_eNextLevel &&
			LEVEL::LEVEL_LOADING	!= m_eNextLevel &&
			LEVEL::LEVEL_LOGO		!= m_eNextLevel &&
			LEVEL::LEVEL_LOBBY      != m_eNextLevel &&
			LEVEL::LEVEL_EDIT       != m_eNextLevel)
		{
			m_pUILoading.lock()->Set_Complete();

			if (m_pUILoading.lock()->Get_Finish())
				Create_Level();
		}
		else
		{
			Create_Level();
		}
	}

	if (KEY_INPUT(KEY::HOME, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Write_JsonUsingResource("../Bin/LevelData/CapturedResource/Loading.json");
	}
}

HRESULT CLevel_Loading::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(__super::Render(pDeviceContext)))
		return E_FAIL;

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());

	return S_OK;
}

void CLevel_Loading::Create_Level()
{
	shared_ptr<CLevel>	pLevel;

	switch (m_eNextLevel)
	{
		case LEVEL_LOGO:
			pLevel = CLevel_Logo::Create();
			break;

		case LEVEL_LOBBY:
			//pLevel = CLevel_Lobby::Create();
			break;

		case LEVEL_GAMEPLAY:
			pLevel = CLevel_GamePlay::Create();
			break;

		case LEVEL_STAGE2:
			pLevel = CLevel_Stage2::Create();
			break;

		case LEVEL_STAGE3:
			pLevel = CLevel_Stage3::Create();
			break;

		case LEVEL_EDIT:
			pLevel = CLevel_Edit::Create();
			break;

		case LEVEL_TEST:
			pLevel = CLevel_Test::Create();
			break;
	}

	if (nullptr == pLevel)
		return;

	if (FAILED(GAMEINSTANCE->Open_Level(m_eNextLevel, pLevel)))
		return;

	GAMEINSTANCE->LevelEnter();
}

shared_ptr<CLevel_Loading> CLevel_Loading::Create(LEVEL eNextLevel)
{
	shared_ptr<CLevel_Loading>		pInstance = make_shared<CLevel_Loading>();
	pInstance->m_eMyLevel = LEVEL_LOADING;
	GAMEINSTANCE->LevelExit();
	GET_SINGLE(CGameManager)->OnLevelExit();
	pInstance->Initialize(eNextLevel);
	
	return pInstance;
}

void CLevel_Loading::Free()
{
}

