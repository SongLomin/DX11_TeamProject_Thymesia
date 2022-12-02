#include "stdafx.h"
#include "Interaction_Elevator.h"

#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "Model.h"

#include "GameInstance.h"
#include "Level_GamePlay.h"

GAMECLASS_C(CInteraction_Elevator);
CLONE_C(CInteraction_Elevator, CGameObject);

HRESULT CInteraction_Elevator::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_Elevator::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom = Add_Component<CCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pModelCom.lock()->Init_Model("Greenhouse_Elevator01", "");

    return S_OK;
}

HRESULT CInteraction_Elevator::Start()
{
    return __super::Start();
}

void CInteraction_Elevator::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_Elevator::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_Elevator::Render()
{
    SetUp_ShaderResource();

    return S_OK;
}

void CInteraction_Elevator::OnEventMessage(_uint iArg)
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
            if (ImGui::InputFloat("Offset Y", &m_fColliderPosY))
            {
                COLLIDERDESC ColliderDesc;
                ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

                ColliderDesc = m_pColliderCom.lock()->Get_ColliderDesc();
                ColliderDesc.vTranslation.y = m_fColliderPosY;

                m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
                m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
            }
        }
        break;
    }
}

void CInteraction_Elevator::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["ColliderPosY"] = m_fColliderPosY;

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);
}

void CInteraction_Elevator::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    if (In_Json.end() != In_Json.find("ColliderPosY"))
    {
        m_fColliderPosY = In_Json["ColliderPosY"];
    }
    
    SetUpColliderDesc();
}

void CInteraction_Elevator::Act_Interaction()
{
    Weak_Cast<CLevel_GamePlay>(GAMEINSTANCE->Get_CurrentLevel()).lock()->ExitLevel(LEVEL::LEVEL_STAGE2);
}

void CInteraction_Elevator::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return ;

        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            m_iPassIndex = 0;
        }
        else
        {
            if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
                m_iPassIndex = 6;
            else
                m_iPassIndex = 7;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex);
        m_pModelCom.lock()->Render_Mesh(i);
    }
}

void CInteraction_Elevator::SetUpColliderDesc()
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale       = _float3(6.f, 0.f, 0.f);
    ColliderDesc.vTranslation = _float3(2.f, m_fColliderPosY, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}
