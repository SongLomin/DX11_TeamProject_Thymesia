#include "stdafx.h"
#include "Window_Level_Dev.h"
#include "GameInstance.h"
#include "Level_GamePlay.h"
#include "Level_Logo.h"
#include "Level_Test.h"
#include "Level_Loading.h"
#include "Level_Stage2.h"

IMPLEMENT_SINGLETON(CWindow_Level_Dev)

HRESULT CWindow_Level_Dev::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	SetUp_ImGuiDESC("Level_Option", ImVec2(400.f, 200.f), window_flags);

	return S_OK;
}

void CWindow_Level_Dev::Start()
{
}

void CWindow_Level_Dev::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

HRESULT CWindow_Level_Dev::Render(ID3D11DeviceContext* pDeviceContext)
{
	ImGui::PushStyleColor(ImGuiCol_PopupBg, m_BackgroundColor);

	if (!(ImGui::Begin(m_tImGuiDESC.strName.c_str(), 0, m_tImGuiDESC.eWindowFlags)))
	{
		__super::End();
		return S_OK;
	}


	if (ImGui::CollapsingHeader("Open Level", ImGuiTreeNodeFlags_DefaultOpen))
	{

		if (ImGui::Button("Level_Logo"))
		{
			shared_ptr<CLevel_Loading>		pLevel_Loading = CLevel_Loading::Create(LEVEL::LEVEL_LOGO);
			if (nullptr == pLevel_Loading.get())
				return E_FAIL;

			if (FAILED(GAMEINSTANCE->Open_Level(LEVEL_LOADING, pLevel_Loading)))
				return E_FAIL;
		}
		if (ImGui::Button("Level_Test"))
		{
			shared_ptr<CLevel_Loading>		pLevel_Loading = CLevel_Loading::Create(LEVEL::LEVEL_TEST);
			if (nullptr == pLevel_Loading.get())
				return E_FAIL;

			if (FAILED(GAMEINSTANCE->Open_Level(LEVEL_LOADING, pLevel_Loading)))
				return E_FAIL;
		}

		if (ImGui::Button("Level_Stage1"))
		{
			shared_ptr<CLevel_Loading>		pLevel_Loading = CLevel_Loading::Create(LEVEL::LEVEL_GAMEPLAY);
			if (nullptr == pLevel_Loading.get())
				return E_FAIL;

			if (FAILED(GAMEINSTANCE->Open_Level(LEVEL_LOADING, pLevel_Loading)))
				return E_FAIL;
		}

		if (ImGui::Button("Level_Stage2"))
		{
			shared_ptr<CLevel_Loading>		pLevel_Loading = CLevel_Loading::Create(LEVEL::LEVEL_STAGE2);
			if (nullptr == pLevel_Loading.get())
				return E_FAIL;

			if (FAILED(GAMEINSTANCE->Open_Level(LEVEL_LOADING, pLevel_Loading)))
				return E_FAIL;
		}

		if (ImGui::Button("Level_Stage3"))
		{
			shared_ptr<CLevel_Loading>		pLevel_Loading = CLevel_Loading::Create(LEVEL::LEVEL_STAGE3);
			if (nullptr == pLevel_Loading.get())
				return E_FAIL;

			if (FAILED(GAMEINSTANCE->Open_Level(LEVEL_LOADING, pLevel_Loading)))
				return E_FAIL;
		}
	}



	// Draw actual text bounding box, following by marker of our expected limit (should not overlap!)



	//std::system_category().message(hr)
	__super::End();

	return S_OK;
}

void CWindow_Level_Dev::Free()
{
}
