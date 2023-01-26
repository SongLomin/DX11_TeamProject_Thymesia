#include "stdafx.h"
#include "Window_EffectEditerView.h"
#include "GameInstance.h"
#include "EffectGroup.h"
#include "CustomEffectMesh.h"
#include "Window_EffectHierarchyView.h"
#include "Window_AnimationModelView.h"
#include "Window_EffectResourceView.h"
#include "PreViewAnimationModel.h"
#include "Model.h"
#include "Window_AnimationPlayerView.h"
#include "Effect_Rect.h"
#include "ImGui_Manager.h"
#include <imgui_impl_win32.h>

IMPLEMENT_SINGLETON(CWindow_EffectEditerView)

HRESULT CWindow_EffectEditerView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    // window_flags |= ImGuiWindowFlags_NoResize;
    // window_flags |= ImGuiWindowFlags_NoMove;

    m_bEnable = true;
    SetUp_ImGuiDESC("EffectEditer_View", ImVec2(300.f, 500.f), window_flags);


    return S_OK;
}

void CWindow_EffectEditerView::Start()
{
    GET_SINGLE(CWindow_EffectHierarchyView)->CallBack_SelectEffect +=
        bind(&CWindow_EffectEditerView::Call_SetCurrentEffect, this, placeholders::_1, placeholders::_2);

    GET_SINGLE(CWindow_EffectHierarchyView)->CallBack_SelectSound +=
        bind(&CWindow_EffectEditerView::Call_SelectSoundFile, this, placeholders::_1);

    GET_SINGLE(CWindow_AnimationModelView)->CallBack_UpdatePreViewModel +=
        bind(&CWindow_EffectEditerView::Call_UpdatePreViewModel, this);

}

void CWindow_EffectEditerView::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    
#ifdef _DEBUG
    if (KEY_INPUT(KEY::F5, KEY_STATE::TAP))
    {
        if (m_pCurrentEffectGroup.lock())
        {
           //  m_pCurrentEffectGroup.lock()->ReBake_EditParticle();
            m_pCurrentEffectGroup.lock()->Reset_Effects();
        }
        
    }

    if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
    {
        if (KEY_INPUT(KEY::S, KEY_STATE::TAP))
        {
            GET_SINGLE(CImGui_Manager)->Save_EffectJson();
        }
    }
#endif // _DEBUG
}

HRESULT CWindow_EffectEditerView::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(__super::Begin()))
        return E_FAIL;

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("CWindow_EffectEditerView", tab_bar_flags))
    {
        switch (m_eCurrentEffectType)
        {
        case Client::CWindow_EffectEditerView::EFFECTRESOURCE_TYPE::MESH:
            if (ImGui::BeginTabItem("Mesh"))
            {
                Update_MeshInfo();
                ImGui::EndTabItem();
            }
            break;
        case Client::CWindow_EffectEditerView::EFFECTRESOURCE_TYPE::PARTICLE:
            if (ImGui::BeginTabItem("Particle"))
            {
                Update_ParticleInfo();
                ImGui::EndTabItem();
            }
            break;
        case Client::CWindow_EffectEditerView::EFFECTRESOURCE_TYPE::SOUND:
            if (ImGui::BeginTabItem("Sound"))
            {
                Update_Sound();
                ImGui::EndTabItem();
            }
            break;
        case Client::CWindow_EffectEditerView::EFFECTRESOURCE_TYPE::TYPE_END:
            break;
        default:
            break;
        }

        ImGui::EndTabBar();
    }

    __super::End();

    return S_OK;
}

void CWindow_EffectEditerView::Call_SetCurrentEffect(weak_ptr<CEffectGroup> pEffectGroup, _uint iIndex)
{
    m_pCurrentEffectGroup = pEffectGroup;
    m_pCurrentEffectMesh = weak_ptr<CCustomEffectMesh>();
    m_pCurrentEffectParticle = weak_ptr<CEffect_Rect>();

    // EffectMesh
    if (m_pCurrentEffectGroup.lock()->Is_EffectMesh(iIndex))
    {
        m_eCurrentEffectType = EFFECTRESOURCE_TYPE::MESH;
        m_pCurrentEffectMesh = m_pCurrentEffectGroup.lock()->Get_EffectMesh(iIndex);
    }

    // Particle
    // 파티클은 인덱스에서 이펙트 메쉬 사이즈만큼 빼서 접근해야됨.
    else
    {
        m_eCurrentEffectType = EFFECTRESOURCE_TYPE::PARTICLE;
        m_pCurrentEffectParticle = m_pCurrentEffectGroup.lock()->Get_Particle(iIndex - _uint(m_pCurrentEffectGroup.lock()->Get_EffectMeshSize()));
    }

    GET_SINGLE(CWindow_AnimationPlayerView)->OnUpdateSoundFile("");
}


void CWindow_EffectEditerView::Call_SyncAnimation()
{
    if(m_pCurrentEffectGroup.lock())
        m_pCurrentEffectGroup.lock()->Sync_Animation();
}

void CWindow_EffectEditerView::Call_SelectSoundFile(const _char* In_szSoundFileName)
{
    m_eCurrentEffectType = EFFECTRESOURCE_TYPE::SOUND;
    m_szSoundFileName = In_szSoundFileName;
    m_fVolume = 1.f;

    GET_SINGLE(CWindow_AnimationPlayerView)->OnUpdateSoundFile(m_szSoundFileName);
}

void CWindow_EffectEditerView::Call_UpdatePreViewModel()
{
    m_pPreViewModel = GET_SINGLE(CWindow_AnimationModelView)->Get_PreviewAnimModel();

    m_pPreViewModel.lock()->Get_CurrentModel().lock()->CallBack_AnimationEnd -=
        bind(&CWindow_EffectEditerView::Call_SyncAnimation, this);

    m_pPreViewModel.lock()->Get_CurrentModel().lock()->CallBack_AnimationEnd +=
        bind(&CWindow_EffectEditerView::Call_SyncAnimation, this);
}


void CWindow_EffectEditerView::Update_MeshInfo()
{
    if (!m_pCurrentEffectGroup.lock())
        return;

    if (EFFECTRESOURCE_TYPE::MESH != m_eCurrentEffectType)
        return;

    if (ImGui::Button("Remove Effect") || KEY_INPUT(KEY::F1, KEY_STATE::TAP))
    {
        m_pCurrentEffectGroup.lock()->Remove_EffectMesh(m_pCurrentEffectMesh);
        m_pCurrentEffectMesh = weak_ptr<CCustomEffectMesh>();
        m_eCurrentEffectType = EFFECTRESOURCE_TYPE::TYPE_END;
        CallBack_RemoveCurrentEffect();
        return;
    }

    ImGui::SameLine();

#ifdef _DEBUG
    if (ImGui::Button("Reset"))
    {
        m_pCurrentEffectGroup.lock()->Reset_Effects();
    }
#endif // _DEBUG

    _char szName[64];
    strcpy_s(szName, m_pCurrentEffectGroup.lock()->Get_EffectGroupName());

    if (ImGui::InputText("EffectGroupName", szName, 64))
    {
        m_pCurrentEffectGroup.lock()->Set_EffectGroupName(szName);
    }

    m_pCurrentEffectMesh.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW);

}

void CWindow_EffectEditerView::Update_ParticleInfo()
{
    if (!m_pCurrentEffectGroup.lock())
        return;

    if (EFFECTRESOURCE_TYPE::PARTICLE != m_eCurrentEffectType)
    {
        return;
    }

    if (ImGui::Button("Remove Effect"))
    {
        m_pCurrentEffectGroup.lock()->Remove_EffectParticle(m_pCurrentEffectParticle);
        m_pCurrentEffectParticle = weak_ptr<CEffect_Rect>();
        m_eCurrentEffectType = EFFECTRESOURCE_TYPE::TYPE_END;
        CallBack_RemoveCurrentEffect();
        return;
    }

    ImGui::SameLine();

#ifdef _DEBUG
    if (ImGui::Button("Play"))
    {
        m_pCurrentEffectGroup.lock()->Reset_Effects();
    }
#endif // _DEBUG

    ImGui::SameLine();

    if (ImGui::Button("Reset"))
    {
        m_pCurrentEffectGroup.lock()->ReBake_EditParticle();
    }

    _char szName[64];
    strcpy_s(szName, m_pCurrentEffectGroup.lock()->Get_EffectGroupName());

    if (ImGui::InputText("EffectGroupName", szName, 64))
    {
        m_pCurrentEffectGroup.lock()->Set_EffectGroupName(szName);
    }

    m_pCurrentEffectParticle.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EDITDRAW);
}

void CWindow_EffectEditerView::Update_Sound()
{
    string szFrontName = "[" + m_szSoundFileName + "]";

    ImGui::TextColored(ImVec4(1.f, 0.f, 1.f, 1.f), szFrontName.c_str());

    if (ImGui::SliderFloat("Volume", &m_fVolume, 0.f, 5.f, "%.1f"));

    ImGui::SetNextItemWidth(100.f);
    if (ImGui::InputInt("Sync Sound with Key Event", &m_iSoundSyncKey));

    if (ImGui::Button("Play") || KEY_INPUT(KEY::F5, KEY_STATE::TAP))
    {
        GAMEINSTANCE->PlaySound3D(m_szSoundFileName, m_fVolume, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3]);
    }
}


void CWindow_EffectEditerView::Free()
{
}

