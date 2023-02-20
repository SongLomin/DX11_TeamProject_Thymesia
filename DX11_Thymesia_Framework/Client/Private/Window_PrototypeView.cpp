#include "stdafx.h"
#include "Window_PrototypeView.h"
#include "GameObject.h"
#include "Client_GameObjects.h"
#include "Client_Windows.h"

IMPLEMENT_SINGLETON(CWindow_PrototypeView)

HRESULT CWindow_PrototypeView::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
	//window_flags |= ImGuiWindowFlags_NoResize;

	m_bEnable = true;
	SetUp_ImGuiDESC("Prototype_View", ImVec2(300.f, 500.f), window_flags);

    Add_Prototypes();
    GAMEOBJECT_DESC& EditCamera = m_pPrototypes[PROTOTYPE_CAMERA].front();

	return S_OK;
}


void CWindow_PrototypeView::Start()
{
}


void CWindow_PrototypeView::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

HRESULT CWindow_PrototypeView::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	//ImGui::Text("UI_TOOL");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("CWindow_PrototypeView", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Actor"))
        {
            for (auto& elem : m_pPrototypes[PROTOTYPE_ACTOR])
            {
                if (ImGui::Selectable(elem.TypeName.c_str()))
                {
                    CallBack_ListClick(elem.HashCode, elem.TypeName.c_str());
                }
            }
            
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Prop"))
        {
            for (auto& elem : m_pPrototypes[PROTOTYPE_PROP])
            {
                if (ImGui::Selectable(elem.TypeName.c_str()))
                {
                    CallBack_ListClick(elem.HashCode, elem.TypeName.c_str());
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Edit"))
        {
            for (auto& elem : m_pPrototypes[PROTOTYPE_EDIT])
            {
                if (ImGui::Selectable(elem.TypeName.c_str()))
                {
                    CallBack_ListClick(elem.HashCode, elem.TypeName.c_str());
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("UI"))
        {
            for (auto& elem : m_pPrototypes[PROTOTYPE_UI])
            {
                if (ImGui::Selectable(elem.TypeName.c_str()))
                {
                    CallBack_ListClick(elem.HashCode, elem.TypeName.c_str());
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Camera"))
        {
            for (auto& elem : m_pPrototypes[PROTOTYPE_CAMERA])
            {
                if (ImGui::Selectable(elem.TypeName.c_str()))
                {
                    CallBack_ListClick(elem.HashCode, elem.TypeName.c_str());
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("NaviMesh"))
        {
            for (auto& elem : m_pPrototypes[PROTOTYPE_NEVIMESH])
            {
                if (ImGui::Selectable(elem.TypeName.c_str()))
                {
                    CallBack_ListClick(elem.HashCode, elem.TypeName.c_str());
                }
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

	__super::End();

	return S_OK;
}

void CWindow_PrototypeView::Add_Prototypes()
{

#define ADD_PROTOTYPE(Layer, ClassType) m_pPrototypes[Layer].push_back({ typeid(ClassType).hash_code(), typeid(ClassType).name(), GAMEINSTANCE->Add_Prototype_GameObject<ClassType>() });

    ADD_PROTOTYPE(PROTOTYPE_PROP, CTerrain);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CStatic_Prop);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CStatic_Instancing_Prop);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CDynamic_Prop);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CPhysXColliderObject);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CInteraction_Dynamic_Prop);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CInteraction_Ladder);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CInteraction_CheckPoint);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CInteraction_NextPoint);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CInteraction_Elevator);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CInteraction_Door);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CInteraction_Item);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CInteraction_CastleGate);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CWater);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CFog);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CInteriorProp);

    ADD_PROTOTYPE(PROTOTYPE_EDIT, CEditGround);
    ADD_PROTOTYPE(PROTOTYPE_EDIT, CEditGroupProp);
    ADD_PROTOTYPE(PROTOTYPE_EDIT, CEditInstanceProp);
    ADD_PROTOTYPE(PROTOTYPE_EDIT, CEditMapCollider);
    ADD_PROTOTYPE(PROTOTYPE_EDIT, CEditSetActor);
    ADD_PROTOTYPE(PROTOTYPE_EDIT, CEditEventContoller);

    ADD_PROTOTYPE(PROTOTYPE_UI, CFadeMask);

    ADD_PROTOTYPE(PROTOTYPE_CAMERA, CCamera_Free);

    ADD_PROTOTYPE(PROTOTYPE_NEVIMESH, CEditNaviMesh);

#undef ADD_PROTOTYPE(Layer, ClassType)
}


void CWindow_PrototypeView::Free()
{
}
