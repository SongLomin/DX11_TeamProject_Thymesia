#include "stdafx.h"

#include "EditEventContoller.h"
#include "SMath.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_DynamicCube.h"
#include "Shader.h"
#include "Renderer.h"
#include "Model.h"
#include "Collider.h"

#include "Client_GameObjects.h"

#include "GameManager.h";
#include "Window_HierarchyView.h"
#include "ImGui_Window.h"
#include "imGui.h"

GAMECLASS_C(CEditEventContoller)
CLONE_C(CEditEventContoller, CGameObject)

static const char* szEventType[] =
{
	"ON_ENTER_SECTION",
	"ON_EXIT_SECTION",
	"ON_LOCK_SECTION"
};

HRESULT CEditEventContoller::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEditEventContoller::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

HRESULT CEditEventContoller::Start()
{
	return S_OK;
}

void CEditEventContoller::Tick(_float fTimeDelta)
{
}

void CEditEventContoller::LateTick(_float fTimeDelta)
{
}

HRESULT CEditEventContoller::Render(ID3D11DeviceContext* pDeviceContext)
{
	return S_OK;
}

void CEditEventContoller::OnEventMessage(_uint iArg)
{
	switch (iArg)
	{	
		case (_uint)EVENT_TYPE::ON_EDITDRAW:
		{
			if (ImGui::BeginTabBar("Edit"))
			{
				if (ImGui::BeginTabItem("Setting"))
				{
					static const char* szSettingEventList[] =
					{
						"Light",
						"Forg"
					};

					static _int iSelectSettingIndex = 0;

					ImGui::Combo("Select Index", &iSelectSettingIndex, szSettingEventList, IM_ARRAYSIZE(szSettingEventList));

					switch (iSelectSettingIndex)
					{
						case 0 : View_SettingLight(); break;
						case 1 : View_SettingFrog();  break;
					}

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Event"))
				{
					static const char* szEventEventList[] =
					{
						"Light",
						"Section"
					};

					static _int iSelectEventIndex = 0;

					ImGui::Combo("Event", &iSelectEventIndex, szEventEventList, IM_ARRAYSIZE(szEventEventList));
					ImGui::Separator();

					switch (iSelectEventIndex)
					{
						case 0 : View_SelectEventLightType();   break;
						case 1 : View_SelectEventSectionType(); break;
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		break;
	}
}

void CEditEventContoller::View_SettingLight()
{
	static _uint iLightIndex = 0;
}

void CEditEventContoller::View_SettingFrog()
{
	static _float4 vFogColor = { 0.f, 0.f, 0.f, 0.f };
	static _float  fFogRange = 0.f;

	_bool bChange = false;

	bChange |= ImGui::InputFloat4("Fog Color  : ", &vFogColor.x);
	bChange |= ImGui::InputFloat ("Fog Range  : ", &fFogRange);

	if (bChange)
		GAMEINSTANCE->Set_FogDesc(vFogColor, fFogRange);
} 

void CEditEventContoller::View_SelectEventLightType()
{
	ImGui::Combo("Event Type", &iSelectEventType, szEventType, IM_ARRAYSIZE(szEventType));
	ImGui::InputInt("Light Section", &iSelectSection);

	if (ImGui::Button("Act"))
	{
		GET_SINGLE(CGameManager)->Activate_SectionLight(iSelectSection, (EVENT_TYPE)((_uint)EVENT_TYPE::ON_ENTER_SECTION + iSelectEventType));
	}
}

void CEditEventContoller::View_SelectEventSectionType()
{
	ImGui::Combo("Event Type", &iSelectEventType, szEventType, IM_ARRAYSIZE(szEventType));
	ImGui::InputInt("Light Section", &iSelectSection);

	if (ImGui::Button("Act"))
	{
		GET_SINGLE(CGameManager)->Activate_Section(iSelectSection, (EVENT_TYPE)((_uint)EVENT_TYPE::ON_ENTER_SECTION + iSelectEventType));
	}
}

void CEditEventContoller::Free()
{
}