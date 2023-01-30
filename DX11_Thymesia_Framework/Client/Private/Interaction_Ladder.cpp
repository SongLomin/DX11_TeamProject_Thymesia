#include "stdafx.h"
#include "Interaction_Ladder.h"

#include "Model.h"
#include "VIBuffer_Model_Instance.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"

#include "UI_Interaction.h"

#include "GameManager.h"
#include "SMath.h"
#include "imgui.h"


GAMECLASS_C(CInteraction_Ladder);
CLONE_C(CInteraction_Ladder, CGameObject);

#define OFFSET_INDEX 2

HRESULT CInteraction_Ladder::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_Ladder::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pInstanceShaderCom = Add_Component<CShader>();
    m_pMaskingTextureCom = Add_Component<CTexture>();
    m_pUpModelCom        = Add_Component<CModel>();
    m_pInstanceModelCom  = Add_Component<CVIBuffer_Model_Instance>();

    for (_uint i = 0; i < LADDER_COL_TYPE::TYPE_END; ++i)
        m_pColliderCom[i] = Add_Component<CCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pInstanceShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModelInstance"),
        VTXMODEL_INSTANCE_DECLARATION::Element,
        VTXMODEL_INSTANCE_DECLARATION::iNumElements
    );

    m_pMaskingTextureCom.lock()->Use_Texture("UVMask");


    m_pModelCom.lock()->Init_Model("P_Ladder02_Down", "");
    m_pUpModelCom.lock()->Init_Model("P_Ladder02_Up", "");
    m_pInstanceModelCom.lock()->Init_Model("P_Ladder02");

    SetUpColliderDesc();

    m_eInteractionType = INTERACTION_LADDER;

    
    return S_OK;
}

HRESULT CInteraction_Ladder::Start()
{
    __super::Start();

    m_fCullingOffsetRange = 30.f;

    return S_OK;
}

void CInteraction_Ladder::Tick(_float fTimeDelta)
{
    CGameObject::Tick(fTimeDelta);

    if (m_bRenderOutLine)
    {
        // µø¿€

        m_fOutLineBlurIntensity += fTimeDelta * 2.f;
        m_fOutLineBlurIntensity = min(1.f, m_fOutLineBlurIntensity);
    }
    else
    {
        m_fOutLineBlurIntensity -= fTimeDelta * 2.f;
        m_fOutLineBlurIntensity = max(0.f, m_fOutLineBlurIntensity);
    }
}

void CInteraction_Ladder::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_Ladder::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(SetUp_ShaderResource(pDeviceContext)))
        return E_FAIL;

    CGameObject::Render(pDeviceContext);

    return S_OK;
}

HRESULT CInteraction_Ladder::SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(SetUp_ShaderResource_Up(pDeviceContext)))
        return E_FAIL;
    if (FAILED(SetUp_ShaderResource_Mid(pDeviceContext)))
        return E_FAIL;
    if (FAILED(SetUp_ShaderResource_Down(pDeviceContext)))
        return E_FAIL;

    return S_OK;
}

void CInteraction_Ladder::OnEventMessage(_uint iArg)
{
    __super::OnEventMessage(iArg);

    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_EDITINIT:
        {
            UpdateLadder();
        }
        break;

        case EVENT_TYPE::ON_EDIT_UDATE:
        {
            UpdateLadder();
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            _bool bChage = false;

            bChage |= ImGui::DragFloat("Mid Offset", &m_fOffset, 1.f);
            bChage |= ImGui::DragInt("Mid Cnt", &m_iMidSize, 1.f);
            //bChage |= ImGui::Button("UpdatePos", ImVec2(100.f, 25.f));

            if (bChage)
            {
                UpdateLadder();
            }
        }
        break;
    }
}

void CInteraction_Ladder::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["Offset"]  = m_fOffset;
    Out_Json["MidSize"] = m_iMidSize;

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);
}

void CInteraction_Ladder::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    m_fOffset  = In_Json["Offset"];
    m_iMidSize = In_Json["MidSize"];

    if (0 < m_iMidSize)
    {
        vector<INSTANCE_MESH_DESC> Prop_Desc;
        INSTANCE_MESH_DESC Desc;
        ZeroMemory(&Desc, sizeof(INSTANCE_MESH_DESC));

        _float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(m_pTransformCom.lock()->Get_WorldMatrix()));
        _vector vQuaternion = XMQuaternionRotationRollPitchYaw(vPitchYawRoll.x, vPitchYawRoll.y, vPitchYawRoll.z);

        Desc.vScale    = m_pTransformCom.lock()->Get_Scaled();
        Desc.vRotation = vPitchYawRoll;

        _vector vPos = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
        for (_uint i = 0; i < m_iMidSize; ++i)
        {
            XMStoreFloat3(&Desc.vTarnslation, XMVectorSetY(vPos, XMVectorGetY(vPos) + (m_fOffset * (i + OFFSET_INDEX))));
            Prop_Desc.push_back(Desc);
        }

        m_pInstanceModelCom.lock()->Init_Instance(m_iMidSize);
        m_pInstanceModelCom.lock()->Update(Prop_Desc);

        COLLIDERDESC ColliderDesc;
        ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

        ColliderDesc.iLayer = (_uint)COLLISION_LAYER::TRIGGER;
        ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);

        SetUpColliderDesc();
    }
}

void CInteraction_Ladder::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    m_eEnterLadder = ((_uint)COLLISION_LAYER::LADDER_DOWN == pMyCollider.lock()->Get_CollisionLayer())
        ? (LADDER_COL_TYPE::DOWN_USE)
        : (LADDER_COL_TYPE::UP_USE);
}

void CInteraction_Ladder::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CInteraction_Ladder::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CInteraction_Ladder::Set_RenderOutLine(_bool bState)
{
    m_bRenderOutLine = bState;

    if (bState)
    {
        weak_ptr<CUI_Interaction> pUI_Interaction = GAMEINSTANCE->Get_GameObjects<CUI_Interaction>(LEVEL_STATIC).front();

        if (!pUI_Interaction.lock())
            return;

        Callback_ActStart += bind(&CUI_Interaction::Call_ActionStart, pUI_Interaction.lock());
        Callback_ActEnd   += bind(&CUI_Interaction::Call_ActionEnd, pUI_Interaction.lock());

        pUI_Interaction.lock()->Call_CollisionEnter
        (
            m_pColliderCom[m_eEnterLadder],
            (_uint)m_eInteractionType
        );
    }
    else
    {
        weak_ptr<CUI_Interaction> pUI_Interaction = GAMEINSTANCE->Get_GameObjects<CUI_Interaction>(LEVEL_STATIC).front();

        if (!pUI_Interaction.lock())
            return;

        pUI_Interaction.lock()->Call_CollisionExit();

        Callback_ActStart.Clear();
        Callback_ActEnd.Clear();
    }
}

HRESULT CInteraction_Ladder::SetUp_ShaderResource_Up(ID3D11DeviceContext* pDeviceContext)
{
    _float4x4 WorldMatrix;
    XMStoreFloat4x4(&WorldMatrix, m_pTransformCom.lock()->Get_WorldMatrix());
    
    m_fUpLadderHeight = (m_fOffset * (m_iMidSize + OFFSET_INDEX));

    WorldMatrix._42 += m_fUpLadderHeight;

    XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)));

    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;

    _float4 vCamDesc;
    XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3]);
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vCamPosition", &vCamDesc, sizeof(_float4))))
        return E_FAIL;

    XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2]);
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vCamLook", &vCamDesc, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 92)))
        return E_FAIL;

    _vector vShaderFlag = { 1.f, m_fOutLineBlurIntensity, 0.f, 0.f };
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
        return E_FAIL;

    _uint iNumMeshContainers = m_pUpModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _flag BindTextureFlag = 0;

        if (SUCCEEDED(m_pUpModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
        {
            BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
        }

        if (SUCCEEDED(m_pUpModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            BindTextureFlag |= (1 << aiTextureType_NORMALS);
        }

        if (SUCCEEDED(m_pUpModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
        {
            BindTextureFlag |= (1 << aiTextureType_SPECULAR);
        }
        else
        {
            BEGIN_PERFROMANCE_CHECK(m_pModelCom.lock()->Get_ModelKey());
            END_PERFROMANCE_CHECK(m_pModelCom.lock()->Get_ModelKey());
        }
        m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, false, false, false);

        if ((_uint)-1 == m_iPassIndex)
        {
            continue;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pUpModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return S_OK;
}

HRESULT CInteraction_Ladder::SetUp_ShaderResource_Mid(ID3D11DeviceContext* pDeviceContext)
{
    _float4x4 WorldMatrix;
    XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

    if (FAILED(m_pInstanceShaderCom.lock()->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pInstanceShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pInstanceShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;

    _float4 vCamDesc;
    XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3]);
    if (FAILED(m_pInstanceShaderCom.lock()->Set_RawValue("g_vCamPosition", &vCamDesc, sizeof(_float4))))
        return E_FAIL;

    XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2]);
    if (FAILED(m_pInstanceShaderCom.lock()->Set_RawValue("g_vCamLook", &vCamDesc, sizeof(_float4))))
        return E_FAIL;

    _float4 vPlayerPos;
    XMStoreFloat4(&vPlayerPos, GET_SINGLE(CGameManager)->Get_PlayerPos());
    if (FAILED(m_pInstanceShaderCom.lock()->Set_RawValue("g_vPlayerPosition", &vPlayerPos, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pInstanceShaderCom, "g_MaskTexture", 92)))
        return E_FAIL;

    _vector vShaderFlag = { 1.f, m_fOutLineBlurIntensity, 0.f, 0.f };
    if (FAILED(m_pInstanceShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
        return E_FAIL;

    _uint iNumMeshContainers = m_pInstanceModelCom.lock()->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _flag BindTextureFlag = 0;

        if (SUCCEEDED(m_pInstanceModelCom.lock()->Bind_SRV(m_pInstanceShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
        {
            BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
        }

        if (SUCCEEDED(m_pInstanceModelCom.lock()->Bind_SRV(m_pInstanceShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            BindTextureFlag |= (1 << aiTextureType_NORMALS);
        }

        if (SUCCEEDED(m_pInstanceModelCom.lock()->Bind_SRV(m_pInstanceShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
        {
            BindTextureFlag |= (1 << aiTextureType_SPECULAR);
        }
        else
        {
            BEGIN_PERFROMANCE_CHECK(m_pModelCom.lock()->Get_ModelKey());
            END_PERFROMANCE_CHECK(m_pModelCom.lock()->Get_ModelKey());
        }
        m_iPassIndex = Preset::ShaderPass::ModelInstancingShaderPass(BindTextureFlag, false, false, false);

        if ((_uint)-1 == m_iPassIndex)
        {
            continue;
        }
        
        m_pInstanceShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pInstanceModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return S_OK;
}

HRESULT CInteraction_Ladder::SetUp_ShaderResource_Down(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;

    _float4 vCamDesc;
    XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3]);
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vCamPosition", &vCamDesc, sizeof(_float4))))
        return E_FAIL;

    XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2]);
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vCamLook", &vCamDesc, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 92)))
        return E_FAIL;

    _vector vShaderFlag = { 1.f, m_fOutLineBlurIntensity, 0.f, 0.f };
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
        return E_FAIL;

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _flag BindTextureFlag = 0;

        if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
        {
            BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
        }

        if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            BindTextureFlag |= (1 << aiTextureType_NORMALS);
        }

        if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
        {
            BindTextureFlag |= (1 << aiTextureType_SPECULAR);
        }
        else
        {
            BEGIN_PERFROMANCE_CHECK(m_pModelCom.lock()->Get_ModelKey());
            END_PERFROMANCE_CHECK(m_pModelCom.lock()->Get_ModelKey());
        }
        m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, false, false, false);

        if ((_uint)-1 == m_iPassIndex)
        {
            continue;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return S_OK;
}

void CInteraction_Ladder::SetUpColliderDesc()
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    _float3 vUse_Size  = {  1.5f, 0.f, 0.f };
    _float3 vAnim_Size = { 1.5f, 0.f, 0.f };

    ColliderDesc.iLayer = (_uint)COLLISION_LAYER::LADDER_DOWN;
    ColliderDesc.vScale = vUse_Size;

    m_pColliderCom[LADDER_COL_TYPE::DOWN_USE].lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);

    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

   ColliderDesc.iLayer = (_uint)COLLISION_LAYER::LADDER_UP;
   ColliderDesc.vTranslation = { 0.f, m_fOffset * (m_iMidSize + OFFSET_INDEX) + 0.2f, 0.f };
   ColliderDesc.vScale = vUse_Size;
   m_pColliderCom[LADDER_COL_TYPE::UP_USE].lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);

    for (_uint i = 0; i < LADDER_COL_TYPE::TYPE_END; ++i)
        m_pColliderCom[i].lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CInteraction_Ladder::UpdateLadder()
{
    vector<INSTANCE_MESH_DESC> Prop_Desc;
    INSTANCE_MESH_DESC Desc;
    ZeroMemory(&Desc, sizeof(INSTANCE_MESH_DESC));

    _float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(m_pTransformCom.lock()->Get_WorldMatrix()));
    _vector vQuaternion   = XMQuaternionRotationRollPitchYaw(vPitchYawRoll.x, vPitchYawRoll.y, vPitchYawRoll.z);

    Desc.vScale    = m_pTransformCom.lock()->Get_Scaled();
    Desc.vRotation = vPitchYawRoll;

    _vector vPos = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
    for (_uint i = 0; i < m_iMidSize; ++i)
    {
        XMStoreFloat3(&Desc.vTarnslation, XMVectorSetY(vPos, XMVectorGetY(vPos) + (m_fOffset * (i + OFFSET_INDEX))));
        Prop_Desc.push_back(Desc);
    }

    m_pInstanceModelCom.lock()->Init_Instance(m_iMidSize);
    m_pInstanceModelCom.lock()->Update(Prop_Desc);

    SetUpColliderDesc();
}

void CInteraction_Ladder::Free()
{
}
