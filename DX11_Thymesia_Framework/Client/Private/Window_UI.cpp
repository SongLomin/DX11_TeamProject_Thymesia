#include "stdafx.h"
#include "Client_Windows.h"

IMPLEMENT_SINGLETON(CWindow_UI)

HRESULT CWindow_UI::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
	window_flags |= ImGuiWindowFlags_NoResize;
	
	//window_flags |= ImGuiWindowFlags_NoTitleBar;

	m_bEnable = true;
	SetUp_ImGuiDESC("UI_View", ImVec2(300.f, 500.f), window_flags);

	

	return S_OK;
}

void CWindow_UI::Start()
{
}

void CWindow_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

HRESULT CWindow_UI::Render(ID3D11DeviceContext* pDeviceContext)
{
	//if (FAILED(__super::Begin()))
	//	return E_FAIL;

	////ImGui::Text("UI_TOOL");
	//ImGui::PushItemWidth(ImGui::GetFontSize() * -12);


	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("Menu"))
	//	{
	//		if (ImGui::MenuItem("New"))
	//		{
	//		}
	//		if (ImGui::MenuItem("Open", "Ctrl+O"))
	//		{
	//		}
	//		if (ImGui::MenuItem("Save", "Ctrl+S"))
	//		{
	//		}
	//		ImGui::EndMenu();

	//	}
	//	ImGui::EndMenuBar();

	//}




	//__super::End();

	return S_OK;
}

void CWindow_UI::Free()
{
}
