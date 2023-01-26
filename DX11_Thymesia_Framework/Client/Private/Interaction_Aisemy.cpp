#include "stdafx.h"
#include "Interaction_Aisemy.h"

#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "Model.h"
#include "PhysXCollider.h"
#include "Inventory.h"
#include "Item.h"


#include "GameInstance.h"
#include "GameManager.h"
#include "imgui.h"
#include "SMath.h"
#include "UI_ItemRequirement.h"


GAMECLASS_C(CInteraction_Aisemy);
CLONE_C(CInteraction_Aisemy, CGameObject);

HRESULT CInteraction_Aisemy::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_Aisemy::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom            = Add_Component<CCollider>();
    m_pPhysXColliderCom       = Add_Component<CPhysXCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxAnimModel"),
        VTXANIM_DECLARATION::Element,
        VTXANIM_DECLARATION::iNumElements
    );

    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));
    m_eInteractionType = INTERACTION_DOOR;

    m_pModelCom.lock()->Init_Model("NPC_Aisemy");

    _float fDefaultDesc[4] = { 3.5f, -1.f, 1.5f, 0.f };
    SetUpColliderDesc(m_pColliderCom, fDefaultDesc, COLLISION_LAYER::TRIGGER);

    //m_eRenderGroup =

#ifdef _USE_THREAD_
    Use_Thread(THREAD_TYPE::PRE_TICK);
    Use_Thread(THREAD_TYPE::PRE_LATETICK);
#endif // _USE_THREAD_

    return S_OK;
}

HRESULT CInteraction_Aisemy::Start()
{
    __super::Start();

    weak_ptr<MODEL_DATA> pModelData = m_pModelCom.lock()->Get_ModelData();

    if (pModelData.lock())
    {
        m_vCenterOffset       = pModelData.lock()->VertexInfo.vCenter;
        m_fCullingOffsetRange = pModelData.lock()->Get_MaxOffsetRange() * 2.f;
    }

    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

    return S_OK;
}

void CInteraction_Aisemy::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (m_bRendering)
        m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CInteraction_Aisemy::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CInteraction_Aisemy::Thread_PreTick(_float fTimeDelta)
{
    if (!Get_Enable())
        return;

    if (m_bRendering)
        m_pModelCom.lock()->Update_BoneMatrices();
}

void CInteraction_Aisemy::Thread_PreLateTick(_float fTimeDelta)
{
    __super::Thread_PreLateTick(fTimeDelta);

#ifdef _Actor_Culling_
    if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_pTransformCom.lock()->Get_Position(), m_fCullingOffsetRange))
    {
        m_bRendering = true;
    }
    else
    {
        m_bRendering = false;
    }
#else
    m_bRendering = true;
#endif // _Actor_Culling_
}

HRESULT CInteraction_Aisemy::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(SetUp_ShaderResource(pDeviceContext)))
        return E_FAIL;

    CGameObject::Render(pDeviceContext);

    return S_OK;
}

void CInteraction_Aisemy::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_EDIT_UDATE:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
        }
        break;

        static _int   iAnimIndex = 0;
        static _float fAnimSpeed = 1.f;

        if (ImGui::InputInt("AnimIndex", &iAnimIndex))
            m_pModelCom.lock()->Set_CurrentAnimation(iAnimIndex);
        if (ImGui::InputFloat("AnimSpeed", &fAnimSpeed))
            m_pModelCom.lock()->Set_AnimationSpeed(fAnimSpeed);
    }
}

void CInteraction_Aisemy::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);

    //iter = Out_Json["Component"].find("Transfo");

}

void CInteraction_Aisemy::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);
}


void CInteraction_Aisemy::Act_Interaction()
{
    //GET_SINGLE(CGameManager)->
}

HRESULT CInteraction_Aisemy::SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;

    _vector	vShaderFlag = { 0.f,0.f,0.f,0.f };
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
        return E_FAIL;

    _float fCamFar = GAMEINSTANCE->Get_CameraFar();
    m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i(0); i < iNumMeshContainers; ++i)
    {
        m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);
        m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS);

        m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, 4, "g_Bones", pDeviceContext);
    }

    return S_OK;
}

void CInteraction_Aisemy::SetUpColliderDesc(weak_ptr<CCollider> In_pColldierCom, _float* _pColliderDesc, COLLISION_LAYER _eCollLayer)
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)_eCollLayer;
    ColliderDesc.vScale       = _float3(_pColliderDesc[0], 0.f, 0.f); 
    ColliderDesc.vTranslation = _float3(_pColliderDesc[1], _pColliderDesc[2], _pColliderDesc[3]);

    In_pColldierCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    In_pColldierCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CInteraction_Aisemy::SetAnimState(const ANIM_INDEX _eAnimIndex)
{
}

void CInteraction_Aisemy::Free()
{
}
