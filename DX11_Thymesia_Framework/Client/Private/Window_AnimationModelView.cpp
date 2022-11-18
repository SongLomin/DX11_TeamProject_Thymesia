#include "stdafx.h"
#include "Window_AnimationModelView.h"
#include "GameInstance.h"
#include "PreViewAnimationModel.h"
#include "Model.h"
#include "PreView_Prop.h"

IMPLEMENT_SINGLETON(CWindow_AnimationModelView)

weak_ptr<CPreViewAnimationModel> CWindow_AnimationModelView::Get_PreViewModel()
{
    return m_pPreViewModel;
}

HRESULT CWindow_AnimationModelView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    window_flags |= ImGuiWindowFlags_NoResize;

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

HRESULT CWindow_AnimationModelView::Render()
{
    if (FAILED(__super::Begin()))
        return E_FAIL;

    //ImGui::Text("UI_TOOL");
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::CollapsingHeader("Load Model"), ImGuiTreeNodeFlags_DefaultOpen)
        {
            //ImGui::Text("Input Model Key");
            //ImGui::InputText("##ModelKey", m_szModelKey, MAX_PATH);

            ImGui::Text("Model List");
            if (ImGui::BeginListBox("##Model List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
            {

                for (int i = 0; i < m_AllModelKeys.size(); i++)
                {
                    const bool is_selected = (m_CurrentModelIndex == i);
                    if (ImGui::Selectable(m_AllModelKeys[i].c_str(), is_selected))
                        m_CurrentModelIndex = i;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }

            if (ImGui::Button("Load"))
            {
                
                m_pPreViewModel.lock()->Init_EditPreViewAnimationModel(m_AllModelKeys[m_CurrentModelIndex]);
                Update_PreViewModel();
            }

            if (ImGui::CollapsingHeader("Load NoAnimModel"), ImGuiTreeNodeFlags_DefaultOpen)
            {
                //ImGui::Text("Input Model Key");
                //ImGui::InputText("##ModelKey", m_szModelKey, MAX_PATH);

                ImGui::Text(" NoAnimModel List");
                if (ImGui::BeginListBox("## NoAnimModel List", ImVec2(-FLT_MIN, 15 * ImGui::GetTextLineHeightWithSpacing())))
                {

                    for (int i = 0; i < m_AllNoAnimModelKeys.size(); i++)
                    {
                        const bool is_selected = (m_CurrentNoAnimModelIndex == i);
                        if (ImGui::Selectable(m_AllNoAnimModelKeys[i].c_str(), is_selected))
                            m_CurrentNoAnimModelIndex = i;

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndListBox();
                }

                if (ImGui::Button("Load_NonAnim"))
                {

                    m_pPreViewNoAnimModel.lock()->Get_Component<CModel>().lock()->
                        Get_Owner().lock()->
                            Get_Component<CModel>().lock()->Init_Model(m_AllNoAnimModelKeys[m_CurrentModelIndex].c_str());
                   
                }
            }


            //if (m_pPreViewModel.lock())
            //{
            //    if (ImGui::CollapsingHeader("Weapon Collider"), ImGuiTreeNodeFlags_DefaultOpen)
            //    {

            //        ImGui::Text("Bone List");
            //        if (ImGui::BeginListBox("##Bone List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
            //        {

            //            for (int i = 0; i < m_AllBoneNames.size(); i++)
            //            {
            //                const bool is_selected = (m_CurrentBoneIndex == i);
            //                if (ImGui::Selectable(m_AllBoneNames[i].c_str(), is_selected))
            //                    m_CurrentBoneIndex = i;

            //                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            //                if (is_selected)
            //                    ImGui::SetItemDefaultFocus();
            //            }
            //            ImGui::EndListBox();
            //        }

            //        if (ImGui::Button("Create"))
            //        {
            //            m_pPreViewModel.lock()->Add_DebugWeapon(m_AllBoneNames[m_CurrentBoneIndex]);
            //        }

            //        ImGui::SameLine();

            //        if (ImGui::Button("Clear"))
            //        {
            //            m_pPreViewModel.lock()->Clear_DebugWeapon();
            //        }

            //        ImGui::InputFloat("Weapon Scale", &m_tWeaponDesc.fWeaponScale, 0.05f);
            //        ImGui::InputInt("Hit Type", &m_tWeaponDesc.iHitType);
            //        ImGui::InputFloat("Damage", &m_tWeaponDesc.fDamage);
            //        ImGui::InputFloat3("Offset", &m_tWeaponDesc.vWeaponOffset.x, "%.1");
            //    }
            //}
            
        }

        ImGui::EndTabBar();
    }

    __super::End();

    return S_OK;
}

void CWindow_AnimationModelView::Load_PreViewModels()
{
    m_pPreViewModel = GAMEINSTANCE->Add_GameObject<CPreViewAnimationModel>(LEVEL_EDIT);
    m_AllModelKeys = GAMEINSTANCE->Get_AllAnimModelKeys();


    m_pPreViewNoAnimModel = GAMEINSTANCE->Add_GameObject<CPreView_Prop>(LEVEL_EDIT);
    m_AllNoAnimModelKeys = GAMEINSTANCE->Get_AllNoneAnimModelKeys();
}

void CWindow_AnimationModelView::Update_PreViewModel()
{
    CallBack_UpdatePreViewModel();
    m_AllBoneNames = m_pPreViewModel.lock()->Get_CurrentModel().lock()->Get_AllBoneNames();
    m_CurrentBoneIndex = 0;
}


void CWindow_AnimationModelView::Free()
{
    if(m_pPreViewModel.lock())
        m_pPreViewModel.lock()->Set_Dead();

    if (m_pPreViewNoAnimModel.lock())
        m_pPreViewNoAnimModel.lock()->Set_Dead();

}