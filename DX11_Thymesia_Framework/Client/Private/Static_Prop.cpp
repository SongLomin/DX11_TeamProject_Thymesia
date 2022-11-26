#include "stdafx.h"
#include "Static_Prop.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"

#include "PhysXCollider.h"
#include "Client_Presets.h"

GAMECLASS_C(CStatic_Prop);
CLONE_C(CStatic_Prop, CGameObject);

HRESULT CStatic_Prop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStatic_Prop::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pPhyxXColliderCom = Add_Component<CPhysXCollider>();


    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));

    return S_OK;
}

HRESULT CStatic_Prop::Start()
{
    __super::Start();
    
    

    return S_OK;
}

void CStatic_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CStatic_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CStatic_Prop::Before_Render(_float fTimeDelta)
{
    __super::Before_Render(fTimeDelta);
}

HRESULT CStatic_Prop::Render()
{
    __super::Render();

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
        {
            // Do Nothing.
        }

        // 노말인데 5에 저장되어 있다..

        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            // 노말 텍스쳐가 없는 경우
            m_iPassIndex = 0;
        }
        // 노말 텍스쳐가 있는 경우
        else
        {
            m_iPassIndex = 6;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex);
        m_pModelCom.lock()->Render_Mesh(i);
    }

    return S_OK;
}

HRESULT CStatic_Prop::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix)
{
    m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");

    m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
    m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        m_pShaderCom.lock()->Begin(1);
        m_pModelCom.lock()->Render_Mesh(i);
    }

    return S_OK;
}

_bool CStatic_Prop::IsPicking(const RAY& In_Ray, _float& Out_fRange)
{
    RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

    weak_ptr<MODEL_DATA> pModelData = m_pModelCom.lock()->Get_ModelData();

    if (!pModelData.lock())
        return false;

    MESH_VTX_INFO Info = pModelData.lock()->VertexInfo;
    _float fDist;

    if (SMath::Is_Picked_AbstractCube(MouseRayInWorldSpace, Info, m_pTransformCom.lock()->Get_WorldMatrix(), &fDist))
    {
        if (Out_fRange > fDist)
        {
            Out_fRange = fDist;
            return true;
        }
    }

    return false;
}

void CStatic_Prop::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["PhysX_Collider_Type"] = m_iColliderType;
}

void CStatic_Prop::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    if (In_Json.find("PhysX_Collider_Type") != In_Json.end())
    {
        m_iColliderType = In_Json["PhysX_Collider_Type"];
    }

    if ((_uint)LEVEL_EDIT == m_CreatedLevel)
    {
        return;
    }

    if (0 == m_iColliderType)
    {
        return;
    }
    // Model Collider
    else if (1 == m_iColliderType)
    {
        m_pPhyxXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), false);
    }
    // Convex Model_Collider
    else if (2 == m_iColliderType)
    {
        m_pPhyxXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), true);
    }

    PhysXColliderDesc tDesc;
    Preset::PhysXColliderDesc::StaticPropSetting(tDesc, m_pTransformCom);
    m_pPhyxXColliderCom.lock()->CreatePhysXActor(tDesc);
    m_pPhyxXColliderCom.lock()->Add_PhysXActorAtSceneWithOption();

    _vector vMax, vMin;

    vMax = XMLoadFloat3(&m_pModelCom.lock()->Get_ModelData().lock()->VertexInfo.vMax);
    vMin = XMLoadFloat3(&m_pModelCom.lock()->Get_ModelData().lock()->VertexInfo.vMin);
    vMax -= vMin;

    _float fMaxRange = vMax.m128_f32[0];

    for (_uint i = 1; i < 4; ++i)
    {
        if (fMaxRange < vMax.m128_f32[i])
        {
            fMaxRange = vMax.m128_f32[i];
        }
    }

    m_fCullingOffsetRange = fMaxRange;

}

void CStatic_Prop::OnEventMessage(_uint iArg)
{
    __super::OnEventMessage(iArg);

    if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
    {
        const _char* items[] = { "None", "Model", "ConvexModel" };

        if (ImGui::BeginListBox("PhysX Collider Type"))
        {
            for (_uint n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (m_iColliderType == n);
                if (ImGui::Selectable(items[n], is_selected))
                {
                    m_iColliderType = n;
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
    }

}

void CStatic_Prop::Free()
{
}
