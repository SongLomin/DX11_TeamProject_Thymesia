#include "stdafx.h"
#include "Level_Lobby.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "ImGui_Manager.h"
#include "Client_GameObjects.h"
#include "Client_Components.h"
#include "GameManager.h"
#include "Player.h"



GAMECLASS_C(CLevel_Lobby);

HRESULT CLevel_Lobby::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	
	m_eNextLevel = LEVEL_GAMEPLAY;

	//Load_FromJson(m_szDefaultJsonPath + "Lobby.json", LEVEL::LEVEL_LOBBY);

	
	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();


	return S_OK;
}

void CLevel_Lobby::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	Call_FadeOutToLevelChange();

	/*if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
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

HRESULT CLevel_Lobby::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(__super::Render(pDeviceContext)))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Thymesia : LOBBY"));
	return S_OK;
}


shared_ptr<CLevel_Lobby> CLevel_Lobby::Create()
{
	shared_ptr<CLevel_Lobby>		pInstance = make_shared<CLevel_Lobby>();
	pInstance->m_eMyLevel = LEVEL_LOBBY;
	pInstance->Initialize();

	return pInstance;
}

void CLevel_Lobby::Free()
{

}

