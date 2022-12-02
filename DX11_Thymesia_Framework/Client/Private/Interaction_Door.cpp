#include "stdafx.h"
#include "Interaction_Door.h"

#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "Model.h"
#include "PhysXCollider.h"

#include "GameInstance.h"

GAMECLASS_C(CInteraction_Door);
CLONE_C(CInteraction_Door, CGameObject);

HRESULT CInteraction_Door::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_Door::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom      = Add_Component<CCollider>();
    m_pPhysXColliderCom = Add_Component<CPhysXCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pModelCom.lock()->Init_Model("Door01_05", "");

    return S_OK;
}

HRESULT CInteraction_Door::Start()
{
    return __super::Start();
}

void CInteraction_Door::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (m_ActionFlag & ACTION_FLAG::ACTIVATE)
    {
        m_fAddRadian += fTimeDelta;

        if (m_ActionFlag & ACTION_FLAG::ROTATION)
            m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), (m_fFirstRadian + m_fAddRadian));
        else
            m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), (m_fFirstRadian + m_fRotationtRadian - m_fAddRadian));

        if (m_fRotationtRadian <= m_fAddRadian)
        {
            m_ActionFlag  ^= ACTION_FLAG::ROTATION;
            m_ActionFlag  ^= ACTION_FLAG::ACTIVATE;
            m_bOnceAct      = false;
            m_fAddRadian    = 0.f;
        }
    }
}

void CInteraction_Door::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_Door::Render()
{
    SetUp_ShaderResource();

    return S_OK;
}

void CInteraction_Door::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_EDITINIT:
        {
            SetUpColliderDesc();
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

            _bool bCheck_ReverseAct = (m_ActionFlag & ACTION_FLAG::ROTATION);
            if (ImGui::Checkbox("Act Reverse", &bCheck_ReverseAct))
                m_ActionFlag ^= ACTION_FLAG::ROTATION;

            ImGui::DragFloat("Rotation", &m_fRotationtRadian);
        }
        break;
    }
}

void CInteraction_Door::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["RotationtRadian"] = m_fRotationtRadian;

    if (m_ActionFlag & ACTION_FLAG::ROTATION)
        Out_Json["ActionFlag"] = ACTION_FLAG::ROTATION;
    else
        Out_Json["ActionFlag"] = 0;
}

void CInteraction_Door::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    if (In_Json.end() != In_Json.find("RotationtRadian"))
        m_fRotationtRadian = In_Json["RotationtRadian"];

    if (In_Json.end() != In_Json.find("ActionFlag"))
        m_ActionFlag = In_Json["ActionFlag"];

    SetUpColliderDesc();

    m_fFirstRadian = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(m_pTransformCom.lock()->Get_WorldMatrix())).y;
    m_pPhysXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), true);
}

void CInteraction_Door::Act_Interaction()
{
    m_ActionFlag |= ACTION_FLAG::ACTIVATE;
}

void CInteraction_Door::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        m_iPassIndex = 3;

        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return;

        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            m_iPassIndex = 0;
        }
        else
        {
            /*if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
                m_iPassIndex = 6;
            else
                m_iPassIndex = 7;*/
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex);
        m_pModelCom.lock()->Render_Mesh(i);
    }
}

void CInteraction_Door::SetUpColliderDesc()
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale       = _float3(3.f, 0.f, 0.f);
    ColliderDesc.vTranslation = _float3(0.f, 1.5f, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}
