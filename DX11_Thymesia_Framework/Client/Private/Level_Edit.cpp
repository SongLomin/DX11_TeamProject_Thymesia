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

	m_pImGui_Manager = make_unique<CImGui_Manager>();
	m_pImGui_Manager->Initialize();
#ifdef _SKYBOX_
	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_GAMEPLAY);
#endif // _SKYBOX_
	return S_OK;
}

void CLevel_Edit::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pImGui_Manager->Tick(fTimeDelta);


}

HRESULT CLevel_Edit::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("에디터 레벨. "));
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

