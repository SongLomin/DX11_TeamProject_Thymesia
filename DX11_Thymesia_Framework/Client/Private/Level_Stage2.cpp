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

	Load_FromJson(m_szDefaultJsonPath + "Stage2-2.json", LEVEL::LEVEL_STAGE2);

	return S_OK;
}

void CLevel_Stage2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	if (KEY_INPUT(KEY::HOME, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Write_JsonUsingResource("../Bin/LevelData/CapturedResource/Stage2.json");
	}
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

