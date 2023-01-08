#include "stdafx.h"
#include "Window_Optimization_Dev.h"
#include "GameInstance.h"
#include "Level_GamePlay.h"
#include "Level_Logo.h"
#include "Level_Test.h"
#include "Level_Loading.h"
#include "Level_Stage2.h"
#include <imgui_impl_win32.h>

IMPLEMENT_SINGLETON(CWindow_Optimization_Dev)

HRESULT CWindow_Optimization_Dev::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	SetUp_ImGuiDESC("Optimization_Option", ImVec2(300.f, 450.f), window_flags);

	return S_OK;
}

void CWindow_Optimization_Dev::Start()
{
}

void CWindow_Optimization_Dev::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);



}

void CWindow_Optimization_Dev::Background_Tick(_float fTimeDelta)
{
	__super::Background_Tick(fTimeDelta);

#ifdef _DEBUG

#endif // _DEBUG



}

HRESULT CWindow_Optimization_Dev::Render(ID3D11DeviceContext* pDeviceContext)
{
	ImGui::PushStyleColor(ImGuiCol_PopupBg, m_BackgroundColor);

	if (!(ImGui::Begin(m_tImGuiDESC.strName.c_str(), 0, m_tImGuiDESC.eWindowFlags)))
	{
		__super::End();
		return S_OK;
	}

#ifdef _DEBUG
	if (ImGui::InputFloat("Instancing Culling Ratio", &m_fInstancingCullingRatio))
	{
		
	}

#endif // _DEBUG

	if (ImGui::Button("(F6) Clear Perfromance Log") || KEY_INPUT(KEY::F6, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Clear_Logs();
	}

	if (ImGui::Button("(F7) Write Perfromance Log") || KEY_INPUT(KEY::F7, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Write_Log();
	}

	//std::system_category().message(hr)
	__super::End();

	return S_OK;
}

void CWindow_Optimization_Dev::Write_Json(json& Out_Json)
{
	
}

void CWindow_Optimization_Dev::Load_FromJson(const json& In_Json)
{

}

void CWindow_Optimization_Dev::Free()
{

}