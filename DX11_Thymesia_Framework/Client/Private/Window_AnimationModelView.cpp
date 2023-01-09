#include "stdafx.h"
#include "Window_AnimationModelView.h"
#include "GameInstance.h"
#include "PreViewAnimationModel.h"
#include "Model.h"
#include "PreView_Prop.h"
#include <imgui_impl_win32.h>
//#include <imgui_impl_win32.h>

IMPLEMENT_SINGLETON(CWindow_AnimationModelView)

weak_ptr<CPreviewAnimationModel> CWindow_AnimationModelView::Get_PreviewAnimModel()
{
    if (m_pPreviewModel.lock())
        return m_pPreviewModel;
    else
        return weak_ptr<CPreviewAnimationModel>();
}

weak_ptr<CPreview_Prop> CWindow_AnimationModelView::Get_PreviewPropModel()
{
    if (m_pPreviewNoAnimModel.lock())
        return m_pPreviewNoAnimModel;
    else
        return weak_ptr<CPreview_Prop>();
}

HRESULT CWindow_AnimationModelView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    // window_flags |= ImGuiWindowFlags_NoResize;
    // window_flags |= ImGuiWindowFlags_NoMove;

    m_bEnable = true;
    SetUp_ImGuiDESC("AnimationModel_View", ImVec2(150.f, 500.f), window_flags);


    Load_PreViewModels();
    return S_OK;
}

void CWindow_AnimationModelView::Start()
{
}

void CWindow_AnimationModelView::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

HRESULT CWindow_AnimationModelView::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(__super::Begin()))
        return E_FAIL;

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("AnimationModelView", tab_bar_flags))
    {
        if (ImGui::CollapsingHeader("Load Model"))
        {
            //ImGui::Text("Input Model Key");
            //ImGui::InputText("##ModelKey", m_szModelKey, MAX_PATH);

            ImGui::Text("Model List");
            if (ImGui::BeginListBox("##Model List", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
            {

                for (int i = 0; i < m_AllModelKeys.size(); i++)
                {
                    const bool is_selected = (m_CurrentModelIndex == i);
                    if (ImGui::Selectable(m_AllModelKeys[i]->c_str(), is_selected))
                        m_CurrentModelIndex = i;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }

            if (ImGui::Button("Load"))
            {

                m_pPreviewModel.lock()->Init_EditPreviewAnimationModel(*m_AllModelKeys[m_CurrentModelIndex]);
                Update_PreViewModel();
            }

            


            if (m_pPreviewModel.lock())
            {
                if (ImGui::CollapsingHeader("Bone Collider"))
                {

                    ImGui::Text("Bone List");

                    static ImGuiTextFilter BoneFilter;
                    ImGui::Text("Search_Bone"); ImGui::SameLine();
                    BoneFilter.Draw("##BoneSearchBar", 250.f);

                    if (ImGui::BeginListBox("##Bone List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
                    {
                        for (int i = 0; i < m_AllBoneNames.size(); i++)
                        {
                            const bool is_selected = (m_CurrentBoneIndex == i);

                            if (BoneFilter.PassFilter(m_AllBoneNames[i].c_str()))
                            {
                                if (ImGui::Selectable(m_AllBoneNames[i].c_str(), is_selected))
                                    m_CurrentBoneIndex = i;
                            }

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndListBox();
                    }

                    ImGui::InputFloat3("Offset", &m_vOffset.x, "%.1f");
                    ImGui::InputFloat("Size", &m_fSize);

                    if (ImGui::Button("Create"))
                    {
                        m_pPreviewModel.lock()->Add_DebugWeapon(m_AllBoneNames[m_CurrentBoneIndex], m_vOffset, m_fSize * 2.f);
                    }

                    if (ImGui::Button("Delete"))
                    {
                        m_pPreviewModel.lock()->Remove_DebugWeapon(m_AllBoneNames[m_CurrentBoneIndex]);
                    }

                    if (ImGui::Button("Clear"))
                    {
                        m_pPreviewModel.lock()->Clear_DebugWeapon();
                    }

                    ImGui::SameLine();

                    /*if (ImGui::Button("Clear"))
                    {
                        m_pPreViewModel.lock()->Clear_DebugWeapon();
                    }*/
                }
            }

        }

        if (ImGui::CollapsingHeader("Load NoAnimModel"))
        {
            //ImGui::Text("Input Model Key");
            //ImGui::InputText("##ModelKey", m_szModelKey, MAX_PATH);

            ImGui::Text(" NoAnimModel List");

            static ImGuiTextFilter ModelFilter;
            ImGui::Text("Search"); ImGui::SameLine();
            ModelFilter.Draw("##NoAnimModelSearchBar", 250.f);

            if (ImGui::BeginListBox("## NoAnimModel List", ImVec2(-FLT_MIN, 15 * ImGui::GetTextLineHeightWithSpacing())))
            {
                for (int i = 0; i < m_AllNoAnimModelKeys.size(); i++)
                {
                    auto ModelKit = m_AllNoAnimModelKeys.at(i);
                    const bool is_selected = (m_CurrentNoAnimModelIndex == i);

                    if (ModelFilter.PassFilter(ModelKit->c_str()))
                    {
                        std::string label = *ModelKit + "##" + std::to_string(i);

                        if (ImGui::Selectable(label.c_str(), is_selected))
                        {
                            m_CurrentNoAnimModelIndex = i;
                        }
                    }
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }

            if (ImGui::Button("Load_NonAnim"))
            {
                // TODO : comment because explode
                m_pPreviewNoAnimModel.lock()->Get_Component<CModel>().lock()->
                    Get_Owner().lock()->
                    Get_Component<CModel>().lock()->Init_Model(m_AllNoAnimModelKeys[m_CurrentNoAnimModelIndex]->c_str());

            }
        }

        ImGui::EndTabBar();
    }

    __super::End();
    return S_OK;
}

void CWindow_AnimationModelView::Load_PreViewModels()
{
    m_pPreviewModel = GAMEINSTANCE->Add_GameObject<CPreviewAnimationModel>(LEVEL_EDIT);
    m_AllModelKeys = GAMEINSTANCE->Get_AllAnimModelKeys();

    //m_pPreviewModel.lock()->Get_Transform()->Set_Position(XMVectorSet(10.f, 0.5f, 10.f, 1.f));
    //m_pPreviewModel.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.3f);

    // TODO : comment because explode
    m_pPreviewNoAnimModel = GAMEINSTANCE->Add_GameObject<CPreview_Prop>(LEVEL_EDIT);
    m_AllNoAnimModelKeys = GAMEINSTANCE->Get_AllNoneAnimModelKeys();
}

void CWindow_AnimationModelView::Update_PreViewModel()
{
    CallBack_UpdatePreViewModel();
    m_AllBoneNames = m_pPreviewModel.lock()->Get_CurrentModel().lock()->Get_AllBoneNames();
    m_CurrentBoneIndex = 0;
}


void CWindow_AnimationModelView::Free()
{
    if (m_pPreviewModel.lock())
        m_pPreviewModel.lock()->Set_Dead();

    // TODO : comment because explode
    if (m_pPreviewNoAnimModel.lock())
        m_pPreviewNoAnimModel.lock()->Set_Dead();

}