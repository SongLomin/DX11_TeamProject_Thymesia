#include "stdafx.h"
#include "DeveloperConsole_Manager.h"
#include "GameInstance.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "Window_Shader_Dev.h"
#include "Window_Level_Dev.h"
#include "Window_RenderTarget_Dev.h"
#include "Window_Optimization_Dev.h"
#include "Window_GameSystem_Dev.h"
#include "Window_Player_Dev.h"


IMPLEMENT_SINGLETON(CDeveloperConsole_Manager)

void CDeveloperConsole_Manager::Initialize()
{
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImGui::StyleColorsLight();

	//ImGuiStyle& style = ImGui::GetStyle();
	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	style.WindowRounding = 0.0f;
	//	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	//}

	//// Setup Platform/Renderer backends
	//ImGui_ImplWin32_Init(g_hWnd);
	//ImGui_ImplDX11_Init(DEVICE, DEVICECONTEXT);

	Init_Windows();
}

void CDeveloperConsole_Manager::Tick(_float fTimeDelta)
{
	for (auto& elem : m_arrWindows)
	{
		if (elem->Get_Enable())
			elem->Tick(fTimeDelta);
	}
}

void CDeveloperConsole_Manager::Background_Tick(_float fTimeDelta)
{
	for (auto& elem : m_arrWindows)
	{
		if (elem->Get_Enable())
			elem->Background_Tick(fTimeDelta);
	}
}

HRESULT CDeveloperConsole_Manager::Render(ID3D11DeviceContext* pDeviceContext)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto& elem : m_arrWindows)
	{
		if (elem->Get_Enable())
			elem->Render(pDeviceContext);
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

void CDeveloperConsole_Manager::Init_Windows()
{
	Release_Windows();

	m_arrWindows.emplace_back(CWindow_Shader_Dev::Create_Instance());
	m_arrWindows.emplace_back(CWindow_Level_Dev::Create_Instance());
	m_arrWindows.emplace_back(CWindow_RenderTarget_Dev::Create_Instance());
	m_arrWindows.emplace_back(CWindow_Optimization_Dev::Create_Instance());
	m_arrWindows.emplace_back(CWindow_GameSystem_Dev::Create_Instance());
	m_arrWindows.emplace_back(CWindow_Player_Dev::Create_Instance());

	for (auto& elem : m_arrWindows)
	{
		elem->Initialize();
	}

	for (auto& elem : m_arrWindows)
	{
		elem->Start();
	}
}

void CDeveloperConsole_Manager::Release_Windows()
{
	for (_uint i = 0; i < m_arrWindows.size(); ++i)
	{
		m_arrWindows[i].reset();
	}

	m_arrWindows.clear();
	CWindow_Shader_Dev::Destroy_Instance();
	CWindow_Level_Dev::Destroy_Instance();
	CWindow_RenderTarget_Dev::Destroy_Instance();
	CWindow_Optimization_Dev::Destroy_Instance();
	CWindow_GameSystem_Dev::Destroy_Instance();
	CWindow_Player_Dev::Destroy_Instance();
}

void CDeveloperConsole_Manager::OnEnableConsole(const _bool In_bEnable)
{
	EVENT_TYPE eType = In_bEnable ? EVENT_TYPE::ON_CONSOLE_ENABLE : EVENT_TYPE::ON_CONSOLE_DISABLE;

	if (In_bEnable)
	{
		//ImGui::GetIO().ClearInputCharacters();
	}

	for (auto& elem : m_arrWindows)
	{
		elem->OnEventMessage((_uint)eType);
	}
}

void CDeveloperConsole_Manager::Free()
{
	//ImGui_ImplDX11_Shutdown();
	//ImGui_ImplWin32_Shutdown();
	// ImGui::DestroyContext();
}
