#include "stdafx.h"
#include "Window_AnimationPlayerView.h"
#include "GameInstance.h"
#include "Window_AnimationModelView.h"
#include "PreViewAnimationModel.h"
#include "Window_EffectHierarchyView.h"
#include "Window_EffectEditerView.h"
#include "Model.h"
#include "Animation.h"
#include "EffectGroup.h"
#include <imgui_impl_win32.h>

IMPLEMENT_SINGLETON(CWindow_AnimationPlayerView)

HRESULT CWindow_AnimationPlayerView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    // window_flags |= ImGuiWindowFlags_NoResize;
    // window_flags |= ImGuiWindowFlags_NoMove;

    m_bEnable = true;
    SetUp_ImGuiDESC("AnimationPlayer_View", ImVec2(300.f, 200.f), window_flags);


    return S_OK;
}

void CWindow_AnimationPlayerView::Start()
{
    m_pPreViewModel = GET_SINGLE(CWindow_AnimationModelView)->Get_PreviewAnimModel();


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

HRESULT CWindow_AnimationPlayerView::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(__super::Begin()))
        return E_FAIL;

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
    m_pCurrentAnimation = pCurrentModel.lock()->Get_CurrentAnimation();
    m_fCurrentTime = m_pCurrentAnimation.lock()->Get_AbsoluteTimeAcc();
    m_fMaxAnimationTime = m_pCurrentAnimation.lock()->Get_AbsoluteDuration();
    Load_KeyEvent();

    // 중복될 수 있으니 기존 Call 함수 제거
    m_pCurrentAnimation.lock()->CallBack_NextChannelKey -= bind(&CWindow_AnimationPlayerView::Call_NextAnimationKey, this, placeholders::_1);
    // 델리게이트에 바인드.
    m_pCurrentAnimation.lock()->CallBack_NextChannelKey += bind(&CWindow_AnimationPlayerView::Call_NextAnimationKey, this, placeholders::_1);

    m_pPreViewModel.lock()->Bind_KeyFrameEvent(m_pCurrentModelCom.lock()->Get_ModelKey());

}

void CWindow_AnimationPlayerView::Call_NextAnimationKey(const _uint& In_Key)
{
    weak_ptr<CEffectGroup> pCurrentEffectGroup = GET_SINGLE(CWindow_EffectHierarchyView)->Get_EffectGroup();

    if (pCurrentEffectGroup.lock())
    {
        //애니메이션 키가 변경되면 리셋을 호출할지 확인한다.
        pCurrentEffectGroup.lock()->OnChangeAnimationKey(In_Key);
    }


    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();


    /*json& SoundJson = m_KeyEventJson["AnimationIndex"]
        [pCurrentModel.lock()->Get_CurrentAnimationIndex()]
    [In_Key];*/

	if (GET_SINGLE(CWindow_EffectEditerView)->Get_SoundSyncKey() == In_Key)
	{
        if (GET_SINGLE(CWindow_EffectEditerView)->Is_Sound3D())
        {
		    GAMEINSTANCE->PlaySound3D(GET_SINGLE(CWindow_EffectEditerView)->Get_SoundFileName(), GET_SINGLE(CWindow_EffectEditerView)->Get_SoundVolume(), {});
        }
        else
        {
            GAMEINSTANCE->PlaySound2D(GET_SINGLE(CWindow_EffectEditerView)->Get_SoundFileName(), GET_SINGLE(CWindow_EffectEditerView)->Get_SoundVolume());
        }
	}

    if (nullptr == m_KeyEventJson["AnimationIndex"]
        [pCurrentModel.lock()->Get_CurrentAnimationIndex()][In_Key])
    {
        return;
    }

    json& KeyFrameJson = m_KeyEventJson["AnimationIndex"][pCurrentModel.lock()->Get_CurrentAnimationIndex()][In_Key];

    if (KeyFrameJson.empty())
    {
        KeyFrameJson = nullptr;
        return;
    }


    if (KeyFrameJson["Sound"] != nullptr)
    {
        json& SoundJson = KeyFrameJson["Sound"];

        if (!SoundJson.empty())
        {
            for (auto& elem : SoundJson)
            {
                GAMEINSTANCE->PlaySound3D(elem["SoundName"], elem["Volume"], {});
            }
        }
    }


    if (KeyFrameJson["RandomSound"] != nullptr)
    {
        json& RandomSoundJson = KeyFrameJson["RandomSound"];

        if (!RandomSoundJson.empty())
        {
            _int iRandomIndex = rand() % RandomSoundJson.size();
            GAMEINSTANCE->PlaySound3D(RandomSoundJson[iRandomIndex]["SoundName"], RandomSoundJson[iRandomIndex]["Volume"], {});
        }
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

void CWindow_AnimationPlayerView::Add_SoundKeyEvent()
{
    if (!m_pPreViewModel.lock())
        return;

    if (m_strSoundFileName.empty())
        return;

    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    _uint iIndex = pCurrentModel.lock()->Get_CurrentAnimationKeyIndex();

    

    m_KeyEventJson["AnimationIndex"]
        [pCurrentModel.lock()->Get_CurrentAnimationIndex()]
    [iIndex]["Sound"].push_back({});

    json& KeySoundJson = m_KeyEventJson["AnimationIndex"]
        [pCurrentModel.lock()->Get_CurrentAnimationIndex()]
    [iIndex]["Sound"].back();

    KeySoundJson["SoundName"] = m_strSoundFileName;
    KeySoundJson["Volume"] = GET_SINGLE(CWindow_EffectEditerView)->Get_SoundVolume();

    /*m_KeyEventJson["AnimationIndex"]
        [pCurrentModel.lock()->Get_CurrentAnimationIndex()]
    [iIndex]["Sound"].back()["Volume"] = GET_SINGLE(CWindow_EffectEditerView)->Get_SoundVolume();*/

    /*m_KeyEventJson["AnimationIndex"]
        [to_string(pCurrentModel.lock()->Get_CurrentAnimationIndex())].emplace_back();*/

    /*m_KeyEventJson["AnimationIndex"]
        [pCurrentModel.lock()->Get_CurrentAnimationIndex()]
    [iIndex]["Sound"] emplace_back(m_strSoundFileName);*/
}

void CWindow_AnimationPlayerView::Add_RandomSoundKeyEvent()
{
    if (!m_pPreViewModel.lock())
        return;

    if (m_strSoundFileName.empty())
        return;

    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    _uint iIndex = pCurrentModel.lock()->Get_CurrentAnimationKeyIndex();



    m_KeyEventJson["AnimationIndex"]
        [pCurrentModel.lock()->Get_CurrentAnimationIndex()]
    [iIndex]["RandomSound"].push_back({});

    json& KeySoundJson = m_KeyEventJson["AnimationIndex"]
        [pCurrentModel.lock()->Get_CurrentAnimationIndex()]
    [iIndex]["RandomSound"].back();

    KeySoundJson["SoundName"] = m_strSoundFileName;
    KeySoundJson["Volume"] = GET_SINGLE(CWindow_EffectEditerView)->Get_SoundVolume();

}

void CWindow_AnimationPlayerView::Save_KeyEvent()
{
    weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

    string szPath = "../Bin/KeyEventData/";

    if (0 < m_strKeyEventFileName.size())
        szPath += m_strKeyEventFileName;
    else
        szPath += pCurrentModel.lock()->Get_ModelKey();

    szPath += ".json";

    if (FAILED(CJson_Utility::Save_Json(szPath.c_str(), m_KeyEventJson)))
        return;
}

HRESULT CWindow_AnimationPlayerView::Load_KeyEvent()
{
    m_pCurrentModelCom = m_pPreViewModel.lock()->Get_CurrentModel();

    string szPath = "../Bin/KeyEventData/";

    if (0 < m_strKeyEventFileName.size())
        szPath += m_strKeyEventFileName;
    else
        szPath += m_pCurrentModelCom.lock()->Get_ModelKey();

    szPath += ".json";

    if (FAILED(CJson_Utility::Load_Json(szPath.c_str(), m_KeyEventJson)))
        return E_FAIL;

    m_KeyEventEffectGroupNames.clear();
    //m_pPreViewModel.lock()->Clear_DebugWeapon();

    // 읽어서 콜라이더 추가
    /*if (m_KeyEventJson.end() != m_KeyEventJson.find("Collider"))
    {
        for (_size_t i = 0; i < m_KeyEventJson["Collider"].size(); ++i)
        {
            if (m_KeyEventJson["Collider"][i].empty())
                continue;

            m_pPreViewModel.lock()->Add_DebugWeapon(m_KeyEventJson["Collider"][i], {0.f, 0.f, 0.f}, 0.1f);
        }
    }*/

    json& KeyJson = m_KeyEventJson["AnimationIndex"][m_pCurrentModelCom.lock()->Get_CurrentAnimationIndex()];

    if (KeyJson.empty())
        return E_FAIL;

    /*for (_size_t i = 0; i < KeyJson.size(); ++i)
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
    }*/

    return S_OK;
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
    //ImGui::SetNextItemWidth(1800.f);
    ImGui::SliderInt("##Animation Time", &iNewKeyIndex, 0, iMaxKeyIndex, "%d", ImGuiSliderFlags_NoInput);

    if (iCurrentKeyIndex != iNewKeyIndex)
    {
        m_pCurrentAnimation.lock()->Set_StartAnimationKey(iNewKeyIndex);
        m_pPreViewModel.lock()->Play_Animation(0);
    }

    ImGui::SameLine();
    ImGui::Text(to_string(iMaxKeyIndex).c_str());

    if (ImGui::Button("Play"))
    {
        m_bStop = !m_bStop;
    }
    ImGui::Separator();

    _char pFileNameBuffer[MAX_PATH];
    strcpy_s(pFileNameBuffer, m_strKeyEventFileName.c_str());
    ImGui::SetNextItemWidth(250.f);
    if (ImGui::InputText("File Name", pFileNameBuffer, MAX_PATH))
        m_strKeyEventFileName = pFileNameBuffer;

    ImGui::SameLine();

    if (ImGui::Button("Save Custom"))
    {
        if (FAILED(Load_KeyEvent()))
        {
            m_KeyEventJson.clear();
        }

		Save_KeyEvent();
    }
    
    ImGui::Separator();

    if (ImGui::Button("Add_Effect"))
    {
        Add_EffectKeyEvent();
        Save_KeyEvent();
        Load_KeyEvent();
    }

    if (ImGui::Button("Enable_Weapon"))
    {
        Add_EnableWeaponEvent(true);
        Save_KeyEvent();
        Load_KeyEvent();
    }

	ImGui::SameLine();

    if (ImGui::Button("Disable_Weapon"))
    {
        Add_EnableWeaponEvent(false);
        Save_KeyEvent();
        Load_KeyEvent();
    }

    if (m_strSoundFileName.empty())
    {
        ImGui::Text("[Select Sound Resource]");

    }
    else
    {
        ImGui::Text(m_strSoundFileName.c_str());
    }


    ImGui::SameLine();

    if (ImGui::Button("Add_Sound"))
    {
        Add_SoundKeyEvent();
        Save_KeyEvent();
        Load_KeyEvent();
    }

    ImGui::SameLine();

    if (ImGui::Button("Add_RandomSound"))
    {
        Add_RandomSoundKeyEvent();
        Save_KeyEvent();
        Load_KeyEvent();
    }

    ImGui::Separator();

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

    

    ImGui::Separator();
    if (ImGui::CollapsingHeader("KeyEvent List"))
    {
        json& KeyJson = m_KeyEventJson["AnimationIndex"][m_pCurrentModelCom.lock()->Get_CurrentAnimationIndex()];

        string szKeyFrameName;
        string szKeyValueTypeName;
        string szKeyValueName;

        _int iIndex = 0;

        for (_size_t i = 0; i < KeyJson.size(); ++i)
        {
            if (KeyJson[i].empty())
            {
                continue;
            }

            szKeyFrameName = "KeyFrame " + to_string(i) + ". ";

            ImGui::SetNextItemOpen(true);
            if (ImGui::TreeNode(szKeyFrameName.c_str()))
            {
                for (auto iter = KeyJson[i].begin(); iter != KeyJson[i].end();)
                {
                    _bool bKeyEventErase = false;

                    szKeyValueTypeName = iter.key();

                    //for(auto iter_EffectName = iter.value().begin();)

                    if (iter.value().empty())
                    {
                        iter.value() = nullptr;
                        iter = KeyJson[i].erase(iter);
                        continue;
                    }


                    if (strcmp(szKeyValueTypeName.c_str(), "EffectName") == 0)
                    {
                        if (ImGui::TreeNode(szKeyValueTypeName.c_str()))
                        {
                            if (iter.value().empty())
                            {
                                iter.value() = nullptr;
                                iter = KeyJson[i].erase(iter);
                                bKeyEventErase = true;
                            }

                            for (auto iter_EffectName = iter.value().begin(); iter_EffectName != iter.value().end();)
                            {
                                _bool bEffectErase = false;

                                szKeyValueName = iter_EffectName.value();
                                if (ImGui::TreeNode(szKeyValueName.c_str()))
                                {
                                    if (ImGui::Button((string("[Remove] ") + to_string(iIndex++)).c_str()))
                                    {
                                        iter_EffectName = iter.value().erase(iter_EffectName);
                                        bEffectErase = true;
                                    }

                                    ImGui::TreePop();
                                }

                                if (!bEffectErase)
                                {
                                    ++iter_EffectName;
                                }
                            }
                            ImGui::TreePop();
                        }
                    }

                    else if (strcmp(szKeyValueTypeName.c_str(), "Enable_Weapon") == 0)
                    {
                        if (ImGui::TreeNode(string(iter.value().get<_bool>() ? "Enable_Weapon [true]" : "Enable_Weapon [false]").c_str()))
                        {
                            if (ImGui::Button((string("[Remove] ") + to_string(iIndex++)).c_str()))
                            {
                                iter.value() = nullptr;
                                iter = KeyJson[i].erase(iter);
                                bKeyEventErase = true;
                            }

                            ImGui::TreePop();
                        }
                    }

                    else if (strcmp(szKeyValueTypeName.c_str(), "Sound") == 0)
                    {
                        if (ImGui::TreeNode(szKeyValueTypeName.c_str()))
                        {
                            json& SoundJson = iter.value();

                            if (SoundJson.empty())
                            {
                                iter.value() = nullptr;
                                iter = KeyJson[i].erase(iter);
                                bKeyEventErase = true;
                            }

                            for (auto iter_Sound = SoundJson.begin(); iter_Sound != SoundJson.end();)
                            {
                                _bool bEffectErase = false;

                                string SoundFileName = iter_Sound.value()["SoundName"].get<string>().c_str();

                                if (ImGui::TreeNode(string(SoundFileName + to_string(iIndex)).c_str()))
                                {
                                    _float Volume = iter_Sound.value()["Volume"];

                                    if (ImGui::SliderFloat((string("[Volume]") + to_string(iIndex)).c_str(), &Volume, 0, 5.f, "%.1f"))
                                    {
                                        iter_Sound.value()["Volume"] = Volume;
                                    }

                                    if (ImGui::Button((string("[Remove] ") + to_string(iIndex)).c_str()))
                                    {
                                        iter_Sound = SoundJson.erase(iter_Sound);
                                        bEffectErase = true;
                                    }

                                    ImGui::TreePop();
                                    ++iIndex;
                                }

                                if (!bEffectErase)
                                {
                                    ++iter_Sound;
                                }
                            }
                            
                            ImGui::TreePop();
                        }
                    }

                    else if (strcmp(szKeyValueTypeName.c_str(), "RandomSound") == 0)
                    {
                        if (ImGui::TreeNode(szKeyValueTypeName.c_str()))
                        {
                            json& SoundJson = iter.value();

                            if (SoundJson.empty())
                            {
                                iter.value() = nullptr;
                                iter = KeyJson[i].erase(iter);
                                bKeyEventErase = true;
                            }

                            for (auto iter_Sound = SoundJson.begin(); iter_Sound != SoundJson.end();)
                            {
                                _bool bEffectErase = false;

                                string SoundFileName = iter_Sound.value()["SoundName"].get<string>().c_str();

                                if (ImGui::TreeNode(string(SoundFileName + to_string(iIndex)).c_str()))
                                {
                                    _float Volume = iter_Sound.value()["Volume"];

                                    if (ImGui::SliderFloat((string("[Volume]") + to_string(iIndex)).c_str(), &Volume, 0, 2.f, "%.1f"))
                                    {
                                        iter_Sound.value()["Volume"] = Volume;
                                    }

                                    if (ImGui::Button((string("[Remove] ") + to_string(iIndex)).c_str()))
                                    {
                                        iter_Sound = SoundJson.erase(iter_Sound);
                                        bEffectErase = true;
                                    }

                                    ImGui::TreePop();
                                    ++iIndex;
                                }

                                if (!bEffectErase)
                                {
                                    ++iter_Sound;
                                }
                            }

                            ImGui::TreePop();
                        }
                    }


                    if (!bKeyEventErase)
                    {
                        ++iter;
                    }

                }
                ImGui::TreePop();
            }
        }
    }

    ImGui::Separator();

    /*if (ImGui::BeginListBox("##KeyEvent List", ImVec2(-FLT_MIN, 4 * ImGui::GetTextLineHeightWithSpacing())))
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
    }*/

    ImGui::Text(m_pPreViewModel.lock()->Get_CurrentModel().lock()->Get_ModelKey());

    /*ImGui::SameLine();

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
    }*/

    if (ImGui::Button("Save KeyEvent"))
    {
        Save_KeyEvent();
    }

    ImGui::Separator();
    ImGui::Separator();

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

        _byte byFlag(0);

        weak_ptr<CModel> pCurrentModel = m_pPreViewModel.lock()->Get_CurrentModel();

        if (m_bHold)
            byFlag = (_byte)ROOTNODE_FLAG::X | (_byte)ROOTNODE_FLAG::Z;

		if (!strcmp(pCurrentModel.lock()->Get_ModelKey(), "Corvus"))
		{
            pCurrentModel.lock()->Set_RootNode("root_$AssimpFbx$_Translation", byFlag);
        }
        else
        {
			pCurrentModel.lock()->Set_RootNode("root", byFlag);
		}
    }

    ImGui::SameLine();

    ImGui::DragFloat("Animation Speed", &m_fSpeed, 0.01f, 0.f, 2.f);
    GAMEINSTANCE->Set_TimeScale((_uint)TIMESCALE_LAYER::EDITER, m_fSpeed);

    if (ImGui::CollapsingHeader("Select Animation"))
    {
		static ImGuiTextFilter AnimationFilter;
		ImGui::Text("Search"); ImGui::SameLine();
		AnimationFilter.Draw("##AnimationSearchFilter", 340.f);

        if (ImGui::BeginListBox("##Animation List", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
        {
            for (int i = 0; i < m_AllAnimationKeys.size(); i++)
            {
                auto AnimationKit = m_AllAnimationKeys.at(i);
                const bool is_selected = (m_CurrentAnimationIndex == i);

                if (AnimationFilter.PassFilter(AnimationKit.c_str()))
                {
                    std::string label = AnimationKit + "##" + std::to_string(i);

					if (ImGui::Selectable(m_AllAnimationKeys[i].c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick))
					{
						m_CurrentAnimationIndex = i;

						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							m_pPreViewModel.lock()->Change_AnimationFromIndex(m_CurrentAnimationIndex);
							Call_UpdatePreViewModel();
						}
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

void CWindow_AnimationPlayerView::OnUpdateSoundFile(const string& In_szFileName)
{
    m_strSoundFileName = In_szFileName;
}


void CWindow_AnimationPlayerView::Free()
{
}

