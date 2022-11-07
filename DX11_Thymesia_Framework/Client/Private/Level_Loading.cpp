#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Level_Edit.h"
#include "Level_Lobby.h"
#include "Level_Stage2.h"
#include "Level_Stage3.h"
#include "GameInstance.h"

CLevel_Loading::CLevel_Loading()
	//: CLevel(pDevice, pContext) ID3D11Device* pDevice, ID3D11DeviceContext* pContext
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	
//	GAMEINSTANCE->Load_Textures(TEXT("Default"), TEXT("../Bin/Resources/Textures/Default%d.jpg"), MEMORY_TYPE::MEMORY_DYNAMIC);
//	GAMEINSTANCE->Load_Textures(TEXT("LobbyBackground"), TEXT("../Bin/Resources/Textures/Background/BgLobby.png"), MEMORY_TYPE::MEMORY_STATIC);
	//GAMEINSTANCE->Load_Textures(TEXT("Default"), TEXT("../Bin/Resources/Textures/Default%d.jpg"), MEMORY_TYPE::MEMORY_DYNAMIC);
	//GAMEINSTANCE->Load_Textures(TEXT("Background"), TEXT("../Bin/Resources/Textures/Background/BgFightLoading%d.png"), MEMORY_TYPE::MEMORY_STATIC);

	//GAMEINSTANCE->Add_GameObject<CBackGround>(LEVEL_LOADING);

	m_eNextLevel = eNextLevel;
	
	m_pLoader = CLoader::Create(eNextLevel);
	if (nullptr == m_pLoader.get())
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	if (true == m_pLoader->is_Finished())
	{
		shared_ptr<CLevel>	pLevel;

		switch (m_eNextLevel)
		{
		case LEVEL_LOGO:
			
			pLevel = CLevel_Logo::Create();
			break;

		case LEVEL_LOBBY:
			pLevel = CLevel_Lobby::Create();
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

		}

		if (nullptr == pLevel)
			return;


		if (FAILED(GAMEINSTANCE->Open_Level(m_eNextLevel, pLevel)))
			return;

		GAMEINSTANCE->LevelEnter();
	}
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());

	return S_OK;
}

shared_ptr<CLevel_Loading> CLevel_Loading::Create(LEVEL eNextLevel)
{
	shared_ptr<CLevel_Loading>		pInstance = make_shared<CLevel_Loading>();
	pInstance->m_eMyLevel = LEVEL_LOADING;
	GAMEINSTANCE->LevelExit();
	pInstance->Initialize(eNextLevel);
	
	return pInstance;
}

void CLevel_Loading::Free()
{
}

