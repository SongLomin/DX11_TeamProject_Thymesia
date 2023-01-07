#include "stdafx.h"
#include "Window_Shader_Dev.h"
#include "GameInstance.h"
#include "Engine_Struct.h"
#include <imgui_impl_win32.h>
IMPLEMENT_SINGLETON(CWindow_Shader_Dev)

HRESULT CWindow_Shader_Dev::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	SetUp_ImGuiDESC("Shader_Option", ImVec2(400.f, 200.f), window_flags);

	return S_OK;
}

void CWindow_Shader_Dev::Start()
{
} 

void CWindow_Shader_Dev::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
}

HRESULT CWindow_Shader_Dev::Render(ID3D11DeviceContext* pDeviceContext)
{
	ImGui::PushStyleColor(ImGuiCol_PopupBg, m_BackgroundColor);

	if (!(ImGui::Begin(m_tImGuiDESC.strName.c_str(), 0, m_tImGuiDESC.eWindowFlags)))
	{
		__super::End();
		return S_OK;
	}

	ImGui::PushID("Rebuild Changed Shaders");
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.8f));
	if (ImGui::Button("(F5) Rebuild Changed Shaders") || KEY_INPUT(KEY::F5, KEY_STATE::TAP))
	{
		m_szCompileMessage.clear();
		GAMEINSTANCE->ReLoad_AllShader(m_szCompileMessage);
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	static float wrap_width = 385.f;
	ImGui::SliderFloat("Debug Log width", &wrap_width, -20, 600, "%.0f");

	if (ImGui::CollapsingHeader("Shader Logs"))
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		for (auto& elem : m_szCompileMessage)
		{
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 marker_min = ImVec2(pos.x + wrap_width, pos.y);
			ImVec2 marker_max = ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight());
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);

			ImVec4 vColor = elem.first ? ImVec4(0.f, 0.f, 0.f, 1.f) : ImVec4(0.6f, 0.f, 0.f, 1.f);

			ImGui::TextColored(vColor, elem.second.c_str(), wrap_width);
			ImGui::Separator();
			//ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "This paragraph should fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.", wrap_width);
		}
	}

	
	
	
	if (ImGui::CollapsingHeader("LiftGammaGain"))
	{
		
		ImGui::DragFloat4("Lift", &m_vLiftColor.x, 0.01f, 0.f, 10.f, "%.3f");
		ImGui::DragFloat4("Gamma", &m_vGammaColor.x, 0.01f, 0.f, 10.f, "%.3f");
		ImGui::DragFloat4("Gain", &m_vGainColor.x, 0.01f, 0.f, 10.f, "%.3f");
	
		if (ImGui::Button("Update LiftGammaGain"))
		{
			GAMEINSTANCE->Set_LiftGammaGain(m_vLiftColor, m_vGammaColor, m_vGainColor);
		}
	}

	if (ImGui::CollapsingHeader("Fog"))
	{
		ImGui::ColorPicker4("FogColor", &m_vFogColor.x, ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_InputRGB);

		ImGui::DragFloat("Fog Range", &m_fFogRange, 1.f, 0.f, 100.f, "%.1f");

		if (ImGui::Button("Update Fog"))
		{
			GAMEINSTANCE->Set_FogDesc(m_vFogColor, m_fFogRange);
		}
	}
	
	if (ImGui::CollapsingHeader("Shadow Light"))
	{
		ImGui::DragFloat4("Shadow Eye", &m_vEye.x);
		
		ImGui::DragFloat4("Shadow LookAt", &m_vLookAt.x);

	
		if (ImGui::Button("Update ShadowLight"))
		{
			GAMEINSTANCE->Set_ShadowLight(XMLoadFloat3(&m_vEye), XMLoadFloat3(&m_vLookAt));
		}
	}

	if (ImGui::CollapsingHeader("Screen Tone"))
	{
		ImGui::DragFloat("Contrast", &m_fContrast,0.01f,0.f,10.f,"%.3f");

		if (ImGui::Button("Update Contrast"))
		{
			GAMEINSTANCE->Set_Contrast(m_fContrast);
		}
		ImGui::DragFloat("Saturation", &m_fSaturation, 0.01f, 0.f, 10.f, "%.3f");
		if (ImGui::Button("Update Saturation"))
		{
			GAMEINSTANCE->Set_Saturation(m_fSaturation);
		}
		ImGui::DragFloat("Exposure", &m_fExposure, 0.01f, 0.f, 10.f, "%.3f");
		if (ImGui::Button("Update Exposure"))
		{
			GAMEINSTANCE->Set_Exposure(m_fExposure);
		}
	}

	if (ImGui::CollapsingHeader("Irradiance Color Scale"))
	{
		
		ImGui::ColorPicker3("Irradiance Color", &m_vIrradianceColorScale.x, ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_InputRGB);
	
		if (ImGui::Button("Update Irradiance Color"))
		{
			GAMEINSTANCE->Set_IrradianceColorScale(m_vIrradianceColorScale);
		}
	}

	if (ImGui::CollapsingHeader("Sharpness"))
	{
		ImGui::SliderFloat("Sharpness Ratio", &m_fSharpness, 0.f, 100.f, "%.1f");

		if (ImGui::Button("Update Sharpness"))
		{
			GAMEINSTANCE->Set_Sharpness(m_fSharpness);
		}
	}

	// Draw actual text bounding box, following by marker of our expected limit (should not overlap!)
	
	

	//std::system_category().message(hr)
    __super::End();

	return S_OK;
}

void CWindow_Shader_Dev::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_CONSOLE_ENABLE == iArg)
	{
		m_vFogColor = GAMEINSTANCE->Get_FogColor();
		m_fFogRange = GAMEINSTANCE->Get_FogRange();
		LIFTGAMMAGAIN_DESC LiftGammaGain = GAMEINSTANCE->Get_LiftGammaGain();

		m_vLiftColor = LiftGammaGain.vLift;
		m_vGammaColor = LiftGammaGain.vGamma;
		m_vGainColor = LiftGammaGain.vGain;

		m_vIrradianceColorScale = GAMEINSTANCE->Get_IrradianceColorScale();
	}

	if ((_uint)EVENT_TYPE::ON_CONSOLE_DISABLE == iArg)
	{

	}

}

void CWindow_Shader_Dev::Free()
{
}
