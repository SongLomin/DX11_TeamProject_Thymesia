#include "stdafx.h"
#include "Window_EffectHierarchyView.h"
#include "GameInstance.h"
#include "Window_EffectResourceView.h"
#include "EffectGroup.h"
#include "Window_EffectEditerView.h"
#include <imgui_impl_win32.h>

IMPLEMENT_SINGLETON(CWindow_EffectHierarchyView)

const _char* CWindow_EffectHierarchyView::Get_EffectGroupName() const
{
    if (!m_pEffectGroup.lock())
        return nullptr;

    return m_pEffectGroup.lock()->Get_EffectGroupName();
}

HRESULT CWindow_EffectHierarchyView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    // window_flags |= ImGuiWindowFlags_NoResize;
    // window_flags |= ImGuiWindowFlags_NoMove;

    m_bEnable = true;
    SetUp_ImGuiDESC("Effect_Hierarchy_View", ImVec2(150.f, 500.f), window_flags);


    return S_OK;
}

void CWindow_EffectHierarchyView::Start()
{
    GET_SINGLE(CWindow_EffectResourceView)->CallBack_MeshClick +=
        bind(&CWindow_EffectHierarchyView::Call_AddEffectMesh, this, placeholders::_1);

    GET_SINGLE(CWindow_EffectResourceView)->CallBack_ParticleClick +=
        bind(&CWindow_EffectHierarchyView::Call_AddParticle, this, placeholders::_1, placeholders::_2);

    GET_SINGLE(CWindow_EffectResourceView)->CallBack_EffectGroupClick +=
        bind(&CWindow_EffectHierarchyView::Call_LoadEffectGroup, this, placeholders::_1);

    GET_SINGLE(CWindow_EffectResourceView)->CallBack_SoundFileClick +=
        bind(&CWindow_EffectHierarchyView::Call_LoadSoundFile, this, placeholders::_1);

    GET_SINGLE(CWindow_EffectEditerView)->CallBack_RemoveCurrentEffect +=
        bind(&CWindow_EffectHierarchyView::Call_RemoveCurrentIndex, this);


}

void CWindow_EffectHierarchyView::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

HRESULT CWindow_EffectHierarchyView::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(__super::Begin()))
        return E_FAIL;

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("CWindow_EffectHierarchyView", tab_bar_flags))
    {
        switch (m_eCurrentResourceType)
        {
        case Client::CWindow_EffectHierarchyView::EFFECTRESOURCE_TYPE::EFFECT:
            Render_Effect();
            break;
        case Client::CWindow_EffectHierarchyView::EFFECTRESOURCE_TYPE::SOUND:
            Render_Sound();
            break;
        case Client::CWindow_EffectHierarchyView::EFFECTRESOURCE_TYPE::TYPE_END:
            break;
        default:
            break;
        }

        if (!m_pEffectGroup.lock())
        {

        }

        ImGui::EndTabBar();
    }

    __super::End();

    return S_OK;
}

void CWindow_EffectHierarchyView::Write_FromBinary()
{
    m_pEffectGroup.lock()->Write_FromBinary();
}

void CWindow_EffectHierarchyView::Write_Json(json& Out_Json)
{
    json NewJson;

    m_pEffectGroup.lock()->Write_EffectJson(NewJson);

    string Path = "../Bin/EffectData/";
    Path += m_pEffectGroup.lock()->Get_EffectGroupName();
    Path += ".json";

    CJson_Utility::Save_Json(Path.c_str(), NewJson);

}

void CWindow_EffectHierarchyView::Load_FromJson(const json& In_Json)
{
}

void CWindow_EffectHierarchyView::Call_AddEffectMesh(const _char* In_szModelKey)
{
    m_eCurrentResourceType = EFFECTRESOURCE_TYPE::EFFECT;

    // 모델 키 받아와서 이펙트 메쉬 추가하고 선택

    // 이펙트메쉬그룹이 없으면 만들어서 이펙트 메쉬 추가
    if (!m_pEffectGroup.lock())
    {
        m_pEffectGroup = GAMEINSTANCE->Add_GameObject<CEffectGroup>(LEVEL_EDIT);
    }

    EFFECTMESH_DESC EffectMeshDesc;

    ZeroMemory(&EffectMeshDesc, sizeof(EffectMeshDesc));
#ifdef _DEBUG
    EffectMeshDesc.bOnFocus          = false;
    EffectMeshDesc.fSpeedPerSec      = 1.f;
#endif // _DEBUG

    EffectMeshDesc.fInitTime         = 0.f;
    EffectMeshDesc.fLifeTime         = 9999.f;

    EffectMeshDesc.iSyncAnimationKey = 0;

    EffectMeshDesc.vStartPosition    = { 0.f, 1.f, 0.f };

    EffectMeshDesc.vStartScale       = { 1.f, 1.f, 1.f };
    EffectMeshDesc.vMaxScale         = { 1.f, 1.f, 1.f };

    EffectMeshDesc.vStartColor       = { 1.f, 1.f, 1.f, 1.f };
    EffectMeshDesc.vMaxColor         = { 1.f, 1.f, 1.f, 1.f };

    EffectMeshDesc.bDiffuseWrap      = true;
    EffectMeshDesc.bNoiseWrap        = true;
    EffectMeshDesc.bMaskWrap         = true;
    EffectMeshDesc.vWrapWeight       = { 1.f, 1.f, 1.f, 1.f };

    EffectMeshDesc.vDiffuseUVMax     = { 1.f, 1.f };
    EffectMeshDesc.vNoiseUVMax       = { 1.f, 1.f };
    EffectMeshDesc.vMaskUVMax        = { 1.f, 1.f };

    Add_EffectMesh_Internal(EffectMeshDesc, In_szModelKey);

    //가장 최근에 집어넣은 인덱스
    m_iCurrentEffectIndex            = _uint(m_szEffectMeshNames.size()) - 1;
    CallBack_SelectEffect(m_pEffectGroup, m_iCurrentEffectIndex);

}

void CWindow_EffectHierarchyView::Call_AddParticle(const _char* In_szName, const _char* In_szTextureKey)
{
    m_eCurrentResourceType = EFFECTRESOURCE_TYPE::EFFECT;

    if (!m_pEffectGroup.lock())
        m_pEffectGroup = GAMEINSTANCE->Add_GameObject<CEffectGroup>(LEVEL_EDIT);

    Add_EffectParticle_Internal(In_szName, In_szTextureKey);

    //가장 최근에 집어넣은 인덱스
    m_iCurrentEffectIndex = _uint(m_szEffectMeshNames.size()) + _uint(m_szEffectParticleNames.size()) - 1;
    CallBack_SelectEffect(m_pEffectGroup, m_iCurrentEffectIndex);
}

void CWindow_EffectHierarchyView::Call_LoadEffectGroup(const _char* In_szEffectGroupName)
{
    m_eCurrentResourceType = EFFECTRESOURCE_TYPE::EFFECT;

    string szPath = "../Bin/EffectData/";
    szPath += In_szEffectGroupName;

    if (m_pEffectGroup.lock())
    {
        m_pEffectGroup.lock()->Remove_AllEffects();
        m_pEffectGroup.lock()->Set_Dead();
    }

    m_szEffectMeshNames.clear();
    m_szEffectParticleNames.clear(); 
    m_iCurrentEffectIndex = 0;

    m_pEffectGroup = GAMEINSTANCE->Add_GameObject<CEffectGroup>(LEVEL_EDIT);

    m_pEffectGroup.lock()->Load_EffectJson(szPath, (_uint)TIMESCALE_LAYER::EDITER, LEVEL_EDIT);

    m_szEffectMeshNames = m_pEffectGroup.lock()->Get_EffectMeshKeys();
    m_szEffectParticleNames = m_pEffectGroup.lock()->Get_EffectParticleKeys();

    CallBack_SelectEffect(m_pEffectGroup, m_iCurrentEffectIndex);

}

void CWindow_EffectHierarchyView::Call_LoadSoundFile(const _char* In_szSoundFileName)
{
    m_eCurrentResourceType = EFFECTRESOURCE_TYPE::SOUND;

    if (m_pEffectGroup.lock())
    {
        m_pEffectGroup.lock()->Remove_AllEffects();
        m_pEffectGroup.lock()->Set_Dead();
    }

    CallBack_SelectSound(In_szSoundFileName);

}

void CWindow_EffectHierarchyView::Call_RemoveCurrentIndex()
{
    _uint iIndex = 0;

    for (auto iter = m_szEffectMeshNames.begin(); iter != m_szEffectMeshNames.end();)
    {
        if (iIndex == m_iCurrentEffectIndex)
        {
            m_szEffectMeshNames.erase(iter);
            return;
        }
        else
        {
            ++iter;
            ++iIndex;
        }
    }

    for (auto iter = m_szEffectParticleNames.begin(); iter != m_szEffectParticleNames.end();)
    {
        if (iIndex == m_iCurrentEffectIndex)
        {
            m_szEffectParticleNames.erase(iter);
            return;
        }
        else
        {
            ++iter;
            ++iIndex;
        }
    }

}

void CWindow_EffectHierarchyView::Add_EffectMesh_Internal(const EFFECTMESH_DESC& In_tEffectMeshDesc, const _char* In_szModelKey)
{
    m_pEffectGroup.lock()->Add_EditEffectMesh(In_tEffectMeshDesc, In_szModelKey);
    m_szEffectMeshNames.emplace_back(In_szModelKey);
}

void CWindow_EffectHierarchyView::Add_EffectParticle_Internal(const _char* In_szName, const _char* In_szModelKey)
{
    m_pEffectGroup.lock()->Add_EditParticle(In_szName, In_szModelKey);
    m_szEffectParticleNames.emplace_back(In_szName);
}

void CWindow_EffectHierarchyView::Render_Effect()
{
    if (ImGui::BeginTabItem(m_pEffectGroup.lock()->Get_EffectGroupName()))
    {
        //static ImGuiFilter filter;
        //ImGui::Text("Search : ");
        //filter.Draw("##EffectGroupSearchBar", 340.f);
        //ImGui::BeginChild("##Effect");
        for (int i(0); i < m_szEffectMeshNames.size(); ++i)
        {
            const bool is_selected = (m_iCurrentEffectIndex == i);
            if (ImGui::Selectable(m_szEffectMeshNames[i].c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick))
            {
                m_iCurrentEffectIndex = i;

                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    CallBack_SelectEffect(m_pEffectGroup, m_iCurrentEffectIndex);
                }
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        for (int i(0); i < m_szEffectParticleNames.size(); ++i)
        {
            const bool is_selected = (m_iCurrentEffectIndex == i + m_szEffectMeshNames.size());
            if (ImGui::Selectable(m_szEffectParticleNames[i].c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick))
            {
                m_iCurrentEffectIndex = i + _uint(m_szEffectMeshNames.size());

                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    CallBack_SelectEffect(m_pEffectGroup, m_iCurrentEffectIndex);
                }
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }


        ImGui::EndTabItem();
    }
}

void CWindow_EffectHierarchyView::Render_Sound()
{
    if (ImGui::BeginTabItem("SoundFile"))
    {

        ImGui::EndTabItem();
    }
}

void CWindow_EffectHierarchyView::Free()
{
}