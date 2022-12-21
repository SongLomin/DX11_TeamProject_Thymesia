#include "stdafx.h"
#include "Window_RenderTarget_Dev.h"
#include "GameInstance.h"
#include "Level_GamePlay.h"
#include "Level_Logo.h"
#include "Level_Test.h"
#include "Level_Loading.h"
#include "Level_Stage2.h"

IMPLEMENT_SINGLETON(CWindow_RenderTarget_Dev)

HRESULT CWindow_RenderTarget_Dev::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	SetUp_ImGuiDESC("RenderTarget_Option", ImVec2(300.f, 450.f), window_flags);

	list<const _tchar*> szSRVNames = GAMEINSTANCE->Get_AllSRVNames();

	

	for (auto& elem : szSRVNames)
	{
		m_szSRVNames.push_back(filesystem::path(elem).string().c_str());
	}

	json LoadedJson;

	if (SUCCEEDED(CJson_Utility::Load_Json("..\\Bin\\DeveloperConsoleData\\RenderTargetView.json", LoadedJson)))
	{
		Load_FromJson(LoadedJson);
	}


 	return S_OK;
}

void CWindow_RenderTarget_Dev::Start()
{
}

void CWindow_RenderTarget_Dev::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWindow_RenderTarget_Dev::Background_Tick(_float fTimeDelta)
{
	__super::Background_Tick(fTimeDelta);

#ifdef _DEBUG
	for (_size_t i = 0; i < m_iRenderTargetIndexs.size(); ++i)
	{
		if (m_iRenderTargetIndexs[i] >= 0)
		{
			GAMEINSTANCE->Add_DebugSRT(filesystem::path(m_szSRVNames[m_iRenderTargetIndexs[i]]).wstring().c_str());
		}
	}
#endif // _DEBUG


	
}

HRESULT CWindow_RenderTarget_Dev::Render(ID3D11DeviceContext* pDeviceContext)
{
	ImGui::PushStyleColor(ImGuiCol_PopupBg, m_BackgroundColor);

	if (!(ImGui::Begin(m_tImGuiDESC.strName.c_str(), 0, m_tImGuiDESC.eWindowFlags)))
	{
		__super::End();
		return S_OK;
	}

#ifdef _DEBUG
	if (ImGui::Checkbox("View OldSchool", &m_bOldSchool))
	{
		GAMEINSTANCE->Set_OldSchoolView(m_bOldSchool);
	}

	string szComboLabel;

	if (ImGui::InputInt2("RenderTarget Size", m_RenderTargetSize))
	{
		_float2 vSize((_float)m_RenderTargetSize[0], (_float)m_RenderTargetSize[1]);

		GAMEINSTANCE->Set_DebugSize(vSize);
	}


	for (_size_t i = 0; i < m_iRenderTargetIndexs.size(); ++i)
	{
		szComboLabel = "View RenderTarget " + to_string(i);

		const char* combo_preview_value = m_iRenderTargetIndexs[i] < 0 ? "" : m_szSRVNames[m_iRenderTargetIndexs[i]].c_str();

		if (ImGui::BeginCombo(szComboLabel.c_str(), combo_preview_value))
		{
			for (_size_t n = 0; n < m_szSRVNames.size(); n++)
			{
				const bool is_selected = (m_iRenderTargetIndexs[i] == n);
				if (ImGui::Selectable(m_szSRVNames[n].c_str(), is_selected))
					m_iRenderTargetIndexs[i] = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}


	}

	if (ImGui::Button("Add Debug RenderTarget"))
	{
		m_iRenderTargetIndexs.push_back(-1);
	}

	if (ImGui::Button("Remove Debug RenderTarget"))
	{
		m_iRenderTargetIndexs.pop_back();
	}

#endif // _DEBUG

	
	//std::system_category().message(hr)
	__super::End();

	return S_OK;
}

void CWindow_RenderTarget_Dev::Write_Json(json& Out_Json)
{
	Out_Json["SizeX"] = m_RenderTargetSize[0];
	Out_Json["SizeY"] = m_RenderTargetSize[1];
	Out_Json["IsOldSchoolView"] = m_bOldSchool;


	for (auto& elem : m_iRenderTargetIndexs)
	{
		Out_Json["RenderTargetIndexs"].push_back(elem);
	}
}

void CWindow_RenderTarget_Dev::Load_FromJson(const json& In_Json)
{
	m_RenderTargetSize[0] = In_Json["SizeX"];
	m_RenderTargetSize[1] = In_Json["SizeY"];
	m_bOldSchool = In_Json["IsOldSchoolView"];

	if (In_Json.find("RenderTargetIndexs") != In_Json.end())
	{
		for (auto& elem : In_Json["RenderTargetIndexs"])
		{
			m_iRenderTargetIndexs.push_back(elem);
		}
	}

	_float2 vSize((_float)m_RenderTargetSize[0], (_float)m_RenderTargetSize[1]);

#ifdef _DEBUG
	GAMEINSTANCE->Set_DebugSize(vSize);
	GAMEINSTANCE->Set_OldSchoolView(m_bOldSchool);
#endif // _DEBUG

}

void CWindow_RenderTarget_Dev::Free()
{
	json WriteJson;
	Write_Json(WriteJson);
	CJson_Utility::Save_Json("..\\Bin\\DeveloperConsoleData\\RenderTargetView.json", WriteJson);
}