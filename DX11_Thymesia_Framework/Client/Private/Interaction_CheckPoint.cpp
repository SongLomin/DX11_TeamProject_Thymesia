#include "stdafx.h"
#include "Interaction_CheckPoint.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "UI_Landing.h"
#include "GameInstance.h"
#include "ClientLevel.h"

GAMECLASS_C(CInteraction_CheckPoint);
CLONE_C(CInteraction_CheckPoint, CGameObject);

HRESULT CInteraction_CheckPoint::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_CheckPoint::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom       = Add_Component<CCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pModelCom.lock()->Init_Model("P_ArchiveChair01", "");

    return S_OK;
}

HRESULT CInteraction_CheckPoint::Start()
{
    return __super::Start();
}

void CInteraction_CheckPoint::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_CheckPoint::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_CheckPoint::Render()
{
    SetUp_ShaderResource();


    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        m_iPassIndex = 3;

        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;

        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            m_iPassIndex = 0;
        }
        else
        {
            /* if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
                 m_iPassIndex = 6;
             else
                 m_iPassIndex = 7;*/
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex);
        m_pModelCom.lock()->Render_Mesh(i);
    }

    return S_OK;
}

void CInteraction_CheckPoint::OnEventMessage(_uint iArg)
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
            _float OffsetY = m_pColliderCom.lock()->Get_ColliderDesc().vTranslation.y;

            if (ImGui::InputFloat("Offset Y", &OffsetY))
            {
                COLLIDERDESC ColliderDesc;
                ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

                ColliderDesc = m_pColliderCom.lock()->Get_ColliderDesc();
                ColliderDesc.vTranslation.y = OffsetY;

                m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
                m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
            }
        }
        break;
    }
}


void CInteraction_CheckPoint::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);
}

void CInteraction_CheckPoint::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    SetUpColliderDesc();
}

void CInteraction_CheckPoint::Act_Interaction()
{
    GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front().lock()->Call_Landing(CUI_Landing::LANDING_BECONFOUND);
   // Weak_StaticCast<CClientLevel>(GAMEINSTANCE->Get_CurrentLevel()).lock()->Call_Enable_EvolveMenu();
}

void CInteraction_CheckPoint::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

}

void CInteraction_CheckPoint::SetUpColliderDesc()
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale = _float3(3.f, 0.f, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}
