#include "stdafx.h"
#include "Window_EffectEditerView.h"
#include "GameInstance.h"
#include "EffectGroup.h"
#include "CustomEffectMesh.h"
#include "Window_EffectHierarchyView.h"
#include "Window_AnimationModelView.h"
#include "PreViewAnimationModel.h"
#include "Model.h"
#include "Window_AnimationPlayerView.h"
#include "Effect_Rect.h"
#include "ImGui_Manager.h"

IMPLEMENT_SINGLETON(CWindow_EffectEditerView)

HRESULT CWindow_EffectEditerView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    window_flags |= ImGuiWindowFlags_NoResize;

    m_bEnable = true;
    SetUp_ImGuiDESC("EffectEditer_View", ImVec2(300.f, 500.f), window_flags);


    return S_OK;
}

void CWindow_EffectEditerView::Start()
{
    GET_SINGLE(CWindow_EffectHierarchyView)->CallBack_SelectEffect +=
        bind(&CWindow_EffectEditerView::Call_SetCurrentEffect, this, placeholders::_1, placeholders::_2);

    GET_SINGLE(CWindow_AnimationModelView)->CallBack_UpdatePreViewModel +=
        bind(&CWindow_EffectEditerView::Call_UpdatePreViewModel, this);

}

void CWindow_EffectEditerView::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    
    if (KEY_INPUT(KEY::F5, KEY_STATE::TAP))
    {
        if (m_pCurrentEffectGroup.lock())
        {
            m_pCurrentEffectGroup.lock()->ReBake_EditParticle();
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

    /*if (GET_SINGLE(CWindow_AnimationPlayerView)->Is_Stop())
        return;

    if (m_pCurrentEffectGroup.lock())
        m_pCurrentEffectGroup.lock()->Play(fTimeDelta * GET_SINGLE(CWindow_AnimationPlayerView)->Get_Speed());*/
}

HRESULT CWindow_EffectEditerView::Render()
{
    if (FAILED(__super::Begin()))
        return E_FAIL;

    //ImGui::Text("UI_TOOL");
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (EFFECTRESOURCE_TYPE::MESH == m_eCurrentEffectType)
        {
            if (ImGui::BeginTabItem("Mesh"))
            {
                Update_MeshInfo();
                ImGui::EndTabItem();
            }
        }

        else
        {
            if (ImGui::BeginTabItem("Particle"))
            {
                Update_ParticleInfo();
                ImGui::EndTabItem();
            }
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
    // ��ƼŬ�� �ε������� ����Ʈ �޽� �����ŭ ���� �����ؾߵ�.
    else
    {
        m_eCurrentEffectType = EFFECTRESOURCE_TYPE::PARTICLE;
        m_pCurrentEffectParticle = m_pCurrentEffectGroup.lock()->Get_Particle(iIndex - _uint(m_pCurrentEffectGroup.lock()->Get_EffectMeshSize()));
    }

}


void CWindow_EffectEditerView::Call_SyncAnimation()
{
    if(m_pCurrentEffectGroup.lock())
        m_pCurrentEffectGroup.lock()->Sync_Animation();
}

void CWindow_EffectEditerView::Call_UpdatePreViewModel()
{
    m_pPreViewModel = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel();

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

    if (ImGui::Button("Reset"))
    {
        m_pCurrentEffectGroup.lock()->Reset_Effects();
    }

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

    if (ImGui::Button("Play"))
    {
        m_pCurrentEffectGroup.lock()->Reset_Effects();
    }

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


void CWindow_EffectEditerView::Free()
{
}

