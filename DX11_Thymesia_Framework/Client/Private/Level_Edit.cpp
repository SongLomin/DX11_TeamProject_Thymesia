#include "stdafx.h"
#include "..\Public\Level_Edit.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "ImGui_Manager.h"
#include "SkyBox.h"

GAMECLASS_C(CLevel_Edit);

HRESULT CLevel_Edit::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	ShowCursor(true);

	m_pImGui_Manager = CImGui_Manager::Create_Instance();
	m_pImGui_Manager->Initialize();

#ifdef _SKYBOX_
	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_GAMEPLAY);
#endif // _SKYBOX_

	

#ifdef _RENDER_FPS_
	m_TextInfo_FPS.bCenterAlign = false;
	m_TextInfo_FPS.bAlways = false;
	m_TextInfo_FPS.fRotation = 0.f;
	XMStoreFloat4(&m_TextInfo_FPS.vColor, XMVectorSet(0.f, 0.8588f, 0.176f, 1.f));
	XMStoreFloat2(&m_TextInfo_FPS.vPosition, XMVectorSet(2.f, 2.f, 0.f, 0.f));
	XMStoreFloat2(&m_TextInfo_FPS.vScale, XMVectorSet(0.55f, 0.55f, 0.f, 0.f));
#endif // _RENDER_FPS_

	GAMEINSTANCE->Set_LiftGammaGain(_float4(1.f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f));
	return S_OK;
}

void CLevel_Edit::Tick(_float fTimeDelta)
{
	m_pImGui_Manager->Tick(fTimeDelta);

#ifdef	_RENDER_FPS_
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("Thymesia : EDIT | %d"), m_iNumRender);
		m_TextInfo_FPS.szText = m_szFPS;
		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}

	++m_iNumRender;
#endif // _RENDER_FPS_
}

HRESULT CLevel_Edit::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(__super::Render(pDeviceContext)))
		return E_FAIL;

#ifdef	_RENDER_FPS_
	SetWindowText(g_hWnd, m_TextInfo_FPS.szText.c_str());
#endif // _RENDER_FPS_

	m_pImGui_Manager->Render();
	return S_OK;
}

shared_ptr<CLevel_Edit> CLevel_Edit::Create()
{
	shared_ptr<CLevel_Edit>		pInstance = make_shared<CLevel_Edit>();
	pInstance->m_eMyLevel = LEVEL_EDIT;
	pInstance->Initialize();

	return pInstance;
}

void CLevel_Edit::Free()
{
	m_pImGui_Manager->Destroy_Instance();
}

