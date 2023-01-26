#include "stdafx.h"
#include "Window_GameSystem_Dev.h"
#include "GameInstance.h"
#include "GameManager.h"	
#include "Camera_Target.h"

IMPLEMENT_SINGLETON(CWindow_GameSystem_Dev)

HRESULT CWindow_GameSystem_Dev::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	SetUp_ImGuiDESC("GameSystem_Option", ImVec2(300.f, 450.f), window_flags);

	return S_OK;
}

void CWindow_GameSystem_Dev::Start()
{
}

void CWindow_GameSystem_Dev::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWindow_GameSystem_Dev::Background_Tick(_float fTimeDelta)
{
	__super::Background_Tick(fTimeDelta);

#ifdef _DEBUG

	if (KEY_INPUT(KEY::CTRL, KEY_STATE::TAP))
	{
		if (KEY_INPUT(KEY::M, KEY_STATE::TAP))
		{
			Apply_CameraShaking();
		}
	}

#endif // _DEBUG



}

HRESULT CWindow_GameSystem_Dev::Render(ID3D11DeviceContext* pDeviceContext)
{
	ImGui::PushStyleColor(ImGuiCol_PopupBg, m_BackgroundColor);

	if (!(ImGui::Begin(m_tImGuiDESC.strName.c_str(), 0, m_tImGuiDESC.eWindowFlags)))
	{
		__super::End();
		return S_OK;
	}

#ifdef _DEBUG
	if (ImGui::CollapsingHeader("Camera Shaking"))
	{
		ImGui::DragFloat3("Shaking_Dir", &m_vShakingDir.x, 0.05f);
		ImGui::SliderFloat("Shaking_Ratio", &m_fShakingRatio, 0.f, 1.f);
		ImGui::DragFloat("Shaking_Time", &m_fShakingTime, 0.01f);
		ImGui::DragFloat("Shaking_Freq", &m_fShakingFrequency, 0.01f);
		ImGui::SliderFloat("Shaking_DecreaseRatio", &m_fShakingDecreaseRatio, 0.f, 1.f);

		if (ImGui::Button("Apply Shaking (M)"))
		{
			Apply_CameraShaking();
		}
	}

#endif // _DEBUG


	//std::system_category().message(hr)
	__super::End();

	return S_OK;
}

void CWindow_GameSystem_Dev::Write_Json(json& Out_Json)
{

}

void CWindow_GameSystem_Dev::Load_FromJson(const json& In_Json)
{

}

void CWindow_GameSystem_Dev::Apply_CameraShaking()
{
	_vector vShakingDirToVector = XMLoadFloat3(&m_vShakingDir);

	GET_SINGLE(CGameManager)->Get_TargetCamera().lock()->Add_Shaking(vShakingDirToVector, 
		m_fShakingRatio, m_fShakingTime, m_fShakingFrequency, m_fShakingDecreaseRatio);
}

void CWindow_GameSystem_Dev::Free()
{

}