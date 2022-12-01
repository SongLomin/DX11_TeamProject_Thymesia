#include "stdafx.h"
#include "ImGui_Window.h"

CImGui_Window::CImGui_Window()
{
}

CImGui_Window::~CImGui_Window()
{
}

void CImGui_Window::Tick(_float fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;
}

HRESULT CImGui_Window::SetUp_ImGuiDESC(string _strName, const ImVec2& _vWindowSize, ImGuiWindowFlags _eWindowFlags)
{
	m_tImGuiDESC.strName = _strName;
	m_tImGuiDESC.vWindowSize = _vWindowSize;
	m_tImGuiDESC.eWindowFlags = _eWindowFlags;

	return S_OK;
}

HRESULT CImGui_Window::Begin()
{
	ImGui::PushStyleColor(ImGuiCol_PopupBg, m_BackgroundColor);

	ImGui::SetNextWindowSize(m_tImGuiDESC.vWindowSize, 0);

	if (!(ImGui::Begin(m_tImGuiDESC.strName.c_str(), 0, m_tImGuiDESC.eWindowFlags)))
		return S_OK;

	/*if (ImGui::IsWindowFocused())
	{
		m_BackgroundColor.w += m_fTimeDelta;
		m_BackgroundColor.w = min(1.f, m_BackgroundColor.w);
	}

	else
	{
		m_BackgroundColor.w -= m_fTimeDelta;
		m_BackgroundColor.w = max(0.f, m_BackgroundColor.w);
	}*/

	return S_OK;
}

HRESULT CImGui_Window::End()
{
	ImGui::PopStyleColor();

	ImGui::End();

	return S_OK;
}
