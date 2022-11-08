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
	window_flags |= ImGuiWindowFlags_NoResize;

	m_bEnable = true;
	SetUp_ImGuiDESC("Prototype_View", ImVec2(300.f, 500.f), window_flags);

    
    Add_Prototypes();


    GAMEOBJECT_DESC& EditCamera = m_pPrototypes[PROTOTYPE_CAMERA].front();
    

	return S_OK;
}

void CWindow_PrototypeView::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

HRESULT CWindow_PrototypeView::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	//ImGui::Text("UI_TOOL");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
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

    //ADD_PROTOTYPE(PROTOTYPE_ACTOR, CDummy_Player);

    ADD_PROTOTYPE(PROTOTYPE_PROP, CTerrain);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CGround);

    /*ADD_PROTOTYPE(PROTOTYPE_PROP, CStage1);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CStage2);
    ADD_PROTOTYPE(PROTOTYPE_PROP, CStage3);*/

    ADD_PROTOTYPE(PROTOTYPE_PROP, CStatic_Prop);

    ADD_PROTOTYPE(PROTOTYPE_UI, CFadeMask);

    ADD_PROTOTYPE(PROTOTYPE_CAMERA, CCamera_Free);

    ADD_PROTOTYPE(PROTOTYPE_NEVIMESH, CEditNaviMesh);
}


void CWindow_PrototypeView::Free()
{
}

void CWindow_PrototypeView::Start()
{
}
