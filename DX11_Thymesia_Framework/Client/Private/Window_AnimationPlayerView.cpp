#include "stdafx.h"
#include "Window_AnimationPlayerView.h"
#include "GameInstance.h"
#include "Window_AnimationModelView.h"
#include "PreViewAnimationModel.h"
#include "Window_EffectHierarchyView.h"
#include "Model.h"
#include "Animation.h"
#include "EffectGroup.h"

IMPLEMENT_SINGLETON(CWindow_AnimationPlayerView)

HRESULT CWindow_AnimationPlayerView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    window_flags |= ImGuiWindowFlags_NoResize;

    m_bEnable = true;
    SetUp_ImGuiDESC("AnimationPlayer_View", ImVec2(300.f, 200.f), window_flags);


    return S_OK;
}

void CWindow_AnimationPlayerView::Start()
{
    m_pPreViewModel = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel();


    GET_SINGLE(CWindow_AnimationModelView)->CallBack_UpdatePreViewModel +=
        bind(&CWindow_AnimationPlayerView::Call_UpdatePreViewModel, this);

}

void CWindow_AnimationPlayerView::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    

    if (!m_bStop)
    {
        if (!m_pPreViewModel.lock())
            return;

        m_pPreViewModel.lock()->Play_Animation(fTimeDelta);
    }
}

HRESULT CWindow_AnimationPlayerView::Render()
{
    if (FAILED(__super::Begin()))
        return E_FAIL;

    //ImGui::Text("UI_TOOL");
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("Animation Player View", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Player"))
        {
            Draw_Player();
            Draw_AnimationList();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Event Editer"))
        {
            Draw_KeyEventEditer();

            ImGui::EndTabItem();
        }

        


        
        
        ImGui::EndTabBar();
    }

    __super::End();

    return S_OK;
}

void CWindow_AnimationPlayerView::Call_UpdatePreViewModel()
{
    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    m_AllAnimationKeys = pCurrentModel.lock()->Get_AllAnimationKeys();
    m_pCurrentAnimation = pCurrentModel.lock()->Get_CurrentAnimation().lock();
    m_fCurrentTime = m_pCurrentAnimation.lock()->Get_AbsoluteTimeAcc();
    m_fMaxAnimationTime = m_pCurrentAnimation.lock()->Get_AbsoluteDuration();
    Load_KeyEvent();

    // 중복될 수 있으니 기존 Call 함수 제거
    m_pCurrentAnimation.lock()->CallBack_NextChannelKey -= bind(&CWindow_AnimationPlayerView::Call_NextAnimationKey, this, placeholders::_1);
    // 델리게이트에 바인드.
    m_pCurrentAnimation.lock()->CallBack_NextChannelKey += bind(&CWindow_AnimationPlayerView::Call_NextAnimationKey, this, placeholders::_1);

}

void CWindow_AnimationPlayerView::Call_NextAnimationKey(const _uint& In_Key)
{
    weak_ptr<CEffectGroup> pCurrentEffectGroup = GET_SINGLE(CWindow_EffectHierarchyView)->Get_EffectGroup();

    if (pCurrentEffectGroup.lock())
    {
        //애니메이션 키가 변경되면 리셋을 호출할지 확인한다.
        pCurrentEffectGroup.lock()->OnChangeAnimationKey(In_Key);
    }

}

void CWindow_AnimationPlayerView::Add_EffectKeyEvent()
{
    if (!m_pPreViewModel.lock())
        return;

    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    const _char* szEffectGroupName = GET_SINGLE(CWindow_EffectHierarchyView)->Get_EffectGroupName();

    if (!szEffectGroupName)
        return;

    _uint iIndex = pCurrentModel.lock()->Get_CurrentAnimationKeyIndex();

    /*m_KeyEventJson["AnimationIndex"]
        [to_string(pCurrentModel.lock()->Get_CurrentAnimationIndex())].emplace_back();*/

    m_KeyEventJson["AnimationIndex"]
        [pCurrentModel.lock()->Get_CurrentAnimationIndex()]
    [iIndex]["EffectName"].emplace_back(szEffectGroupName);

}

void CWindow_AnimationPlayerView::Add_EnableWeaponEvent(const _bool In_bEnable)
{
    if (!m_pPreViewModel.lock())
        return;

    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    _uint iIndex = pCurrentModel.lock()->Get_CurrentAnimationKeyIndex();

    /*m_KeyEventJson["AnimationIndex"]
        [to_string(pCurrentModel.lock()->Get_CurrentAnimationIndex())].emplace_back();*/

    m_KeyEventJson["AnimationIndex"]
        [pCurrentModel.lock()->Get_CurrentAnimationIndex()]
    [iIndex]["Enable_Weapon"] = In_bEnable;
}

void CWindow_AnimationPlayerView::Save_KeyEvent()
{
    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    string szPath = "../Bin/KeyEventData/";
    szPath += pCurrentModel.lock()->Get_ModelKey();
    szPath += ".json";

    if (FAILED(CJson_Utility::Save_Json(szPath.c_str(), m_KeyEventJson)))
    {
        return;
    }

}

void CWindow_AnimationPlayerView::Load_KeyEvent()
{
    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    string szPath = "../Bin/KeyEventData/";
    szPath += pCurrentModel.lock()->Get_ModelKey();
    szPath += ".json";

    if (FAILED(CJson_Utility::Load_Json(szPath.c_str(), m_KeyEventJson)))
        return;

    m_KeyEventEffectGroupNames.clear();
    m_pPreViewModel.lock()->Clear_DebugWeapon();

    // 읽어서 콜라이더 추가
    if (m_KeyEventJson.end() != m_KeyEventJson.find("Collider"))
    {
        for (_size_t i = 0; i < m_KeyEventJson["Collider"].size(); ++i)
        {
            if (m_KeyEventJson["Collider"][i].empty())
                continue;

            m_pPreViewModel.lock()->Add_DebugWeapon(m_KeyEventJson["Collider"][i]);
        }
    }

    json& KeyJson = m_KeyEventJson["AnimationIndex"][pCurrentModel.lock()->Get_CurrentAnimationIndex()];

    if (KeyJson.empty())
        return;

    for (_size_t i = 0; i < KeyJson.size(); ++i)
    {
        if (KeyJson[i].empty())
            continue;

        m_KeyEventEffectGroupNames.emplace(i, list<string>());

        if (KeyJson[i].end() != KeyJson[i].find("Enable_Weapon"))
        {
            string szText = "Weapon(";

            if (KeyJson[i]["Enable_Weapon"] == true)
                szText += "true";
            else
                szText += "false";
            
            szText += ")";
            m_KeyEventEffectGroupNames[(_int)i].emplace_back(szText);
        }

        if (KeyJson[i].end() != KeyJson[i].find("EffectName"))
        {
            for (_size_t j(0); j < KeyJson[i]["EffectName"].size(); ++j)
                m_KeyEventEffectGroupNames[(_int)i].emplace_back(KeyJson[i]["EffectName"][j]);
        }
    }
}

void CWindow_AnimationPlayerView::Clear_KeyEvent()
{
    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    json& KeyJson = m_KeyEventJson["AnimationIndex"][pCurrentModel.lock()->Get_CurrentAnimationIndex()];

    if (KeyJson.empty())
        return;

    _uint iIndex = pCurrentModel.lock()->Get_CurrentAnimationKeyIndex();

    if (KeyJson[iIndex].empty())
        return;

    KeyJson[iIndex]["EffectName"].clear();
}

void CWindow_AnimationPlayerView::ClearBack_KeyEvent()
{
    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    json& KeyJson = m_KeyEventJson["AnimationIndex"][pCurrentModel.lock()->Get_CurrentAnimationIndex()];

    if (KeyJson.empty())
        return;

    _uint iIndex = pCurrentModel.lock()->Get_CurrentAnimationKeyIndex();

    if (KeyJson[iIndex].empty())
        return;

    if (KeyJson[iIndex]["EffectName"].empty())
        return;

    // 뒤에서 한 개 지움.
    KeyJson[iIndex]["EffectName"].erase(KeyJson[iIndex]["EffectName"].size() - 1);
}

void CWindow_AnimationPlayerView::ClearAll_KeyEvent()
{
    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    json& KeyJson = m_KeyEventJson["AnimationIndex"][pCurrentModel.lock()->Get_CurrentAnimationIndex()];

    if (KeyJson.empty())
        return;

    KeyJson.clear();
}

void CWindow_AnimationPlayerView::Clear_WeaponEvent()
{
    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    json& KeyJson = m_KeyEventJson["AnimationIndex"][pCurrentModel.lock()->Get_CurrentAnimationIndex()];

    if (KeyJson.empty())
        return;

    _uint iIndex = pCurrentModel.lock()->Get_CurrentAnimationKeyIndex();

    if (KeyJson[iIndex].empty())
        return;

    KeyJson[iIndex]["Enable_Weapon"].clear();
}

void CWindow_AnimationPlayerView::Draw_Player()
{
    if (!m_pCurrentAnimation.lock())
        return;

    m_fCurrentTime = m_pCurrentAnimation.lock()->Get_AbsoluteTimeAcc();
    ImGui::SliderFloat("##Animation Time", &m_fCurrentTime, 0.0f, m_fMaxAnimationTime, "%.3f");
    m_pCurrentAnimation.lock()->Set_AbsoluteTimeAcc(m_fCurrentTime);
    ImGui::SameLine();
    ImGui::Text(to_string(m_fMaxAnimationTime).c_str());
}

void CWindow_AnimationPlayerView::Draw_KeyEventEditer()
{
    if (!m_pCurrentAnimation.lock())
        return;

    _int iCurrentKeyIndex = (_int)m_pCurrentAnimation.lock()->Get_CurrentChannelKeyIndex();
    _int iNewKeyIndex = iCurrentKeyIndex;
    _int iMaxKeyIndex = (_int)m_pCurrentAnimation.lock()->Get_MaxChannelKeyIndex();

    m_fCurrentTime = m_pCurrentAnimation.lock()->Get_AbsoluteTimeAcc();
    ImGui::SliderInt("##Animation Time", &iNewKeyIndex, 0, iMaxKeyIndex);

    if (iCurrentKeyIndex != iNewKeyIndex)
    {
        m_pCurrentAnimation.lock()->Set_StartAnimationKey(iNewKeyIndex);
        m_pPreViewModel.lock()->Play_Animation(0);
    }

    ImGui::SameLine();
    ImGui::Text(to_string(iMaxKeyIndex).c_str());

    if (ImGui::Button("Add_Effect"))
    {
        Add_EffectKeyEvent();
        Save_KeyEvent();
        Load_KeyEvent();
    }

    ImGui::SameLine();

    if (ImGui::Button("Enable_Weapon"))
    {
        Add_EnableWeaponEvent(true);
        Save_KeyEvent();
        Load_KeyEvent();
    }

    if (ImGui::Button("Disable_Weapon"))
    {
        Add_EnableWeaponEvent(false); 
        Save_KeyEvent();
        Load_KeyEvent();
    }

    //if (ImGui::Button("Clear"))
    //{
    //    Clear_KeyEvent();
    //}
    //ImGui::SameLine();
    //if (ImGui::Button("Clear All"))
    //{
    //    ClearAll_KeyEvent();
    //}
    //ImGui::SameLine();

    if (ImGui::Button("Play"))
    {
        m_bStop = !m_bStop;
    }

    if (ImGui::BeginListBox("##KeyEvent List", ImVec2(-FLT_MIN, 4 * ImGui::GetTextLineHeightWithSpacing())))
    {
        string szListText;

        for (auto& elem_List : m_KeyEventEffectGroupNames)
        {
            szListText = to_string(elem_List.first);
            szListText += ": ";

            for (auto& elem : elem_List.second)
            {
                szListText += elem;
                szListText += ", ";
            }

            const bool is_selected = 0;
            if (ImGui::Selectable(szListText.c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick))
            {
                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    
                }
            }
        }

        ImGui::EndListBox();
    }

    ImGui::Text(m_pPreViewModel.lock()->Get_CurrentModel().lock()->Get_ModelKey());

    ImGui::SameLine();
    
    if (ImGui::Button("Clear Effect Back"))
    {
        ClearBack_KeyEvent();
        Save_KeyEvent();
        Load_KeyEvent();
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear Weapon"))
    {
        Clear_WeaponEvent();
        Save_KeyEvent();
        Load_KeyEvent();
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear All"))
    {
        ClearAll_KeyEvent();
        Save_KeyEvent();
        Load_KeyEvent();
    }

}

void CWindow_AnimationPlayerView::Draw_AnimationList()
{
    if (!m_pCurrentAnimation.lock())
        return;

    if (ImGui::Button("Play"))
    {
        m_bStop = !m_bStop;
    }

    ImGui::SameLine();

    if (ImGui::Button("Hold"))
    {
        m_bHold = !m_bHold;
        weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();
        pCurrentModel.lock()->Set_RootNode("root", m_bHold);
        
    }

    ImGui::SameLine();

    ImGui::DragFloat("Animation Speed", &m_fSpeed, 0.01f, 0.f, 2.f);
    GAMEINSTANCE->Set_TimeScale((_uint)TIMESCALE_LAYER::EDITER, m_fSpeed);

    if (ImGui::CollapsingHeader("Select Animation"), ImGuiTreeNodeFlags_DefaultOpen)
    {
        if (ImGui::BeginListBox("##Animation List", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
        {

            for (int i = 0; i < m_AllAnimationKeys.size(); i++)
            {
                const bool is_selected = (m_CurrentAnimationIndex == i);
                if (ImGui::Selectable(m_AllAnimationKeys[i].c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick))
                {
                    m_CurrentAnimationIndex = i;

                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        m_pPreViewModel.lock()->Change_AnimationFromIndex(m_CurrentAnimationIndex);
                        Call_UpdatePreViewModel();
                    }
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
    }
}


void CWindow_AnimationPlayerView::Free()
{
}

