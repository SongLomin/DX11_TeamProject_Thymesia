#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "Fader.h"
#include "GameManager.h"
#include "Client_GameObjects.h"


CLevel_Logo::CLevel_Logo()
	//: CLevel(pDevice, pContext) ID3D11Device* pDevice, ID3D11DeviceContext* pContext
{

}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eNextLevel = LEVEL_LOBBY;

	m_pFadeMask = GAMEINSTANCE->Add_GameObject<CFadeMask>(LEVEL_STATIC);


	GET_SINGLE(CGameManager)->Set_GameState(GAME_STATE::PEACE);

	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		CFader::FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 1.f;
		tFaderDesc.fDelayTime = 0.5f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_FadeOutToLevelChange, this);
	}


	if (KEY_INPUT(KEY::F2, KEY_STATE::TAP))
	{
		if (FAILED(GAMEINSTANCE->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_EDIT))))
			return;
	}


}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("롷고레벨임. "));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	/* For.BackGround */
	//GAMEINSTANCE->Add_GameObject<CBackGround>(LEVEL_LOGO);
	//GAMEINSTANCE->Add_GameObject<CFadeMask>(LEVEL_STATIC).lock()->Init_Fade(4.f, _float3(0.f, 0.f, 0.f), CFadeMask::FADE_TYPE::SIMPLEX_LNIEAR, true);

	return S_OK;
}

shared_ptr<CLevel_Logo> CLevel_Logo::Create()
{
	shared_ptr<CLevel_Logo>		pInstance = make_shared<CLevel_Logo>();
	pInstance->m_eMyLevel = LEVEL_LOGO;
	pInstance->Initialize();

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}

