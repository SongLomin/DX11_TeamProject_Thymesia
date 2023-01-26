#include "stdafx.h"
#include "Interaction_MapTable.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "ActorDecor.h"
#include "UI_Landing.h"
#include "UIManager.h"
#include "PhysXCollider.h"

#include "GameInstance.h"
#include "ClientLevel.h"
#include "GameManager.h"

GAMECLASS_C(CInteraction_MapTable);
CLONE_C(CInteraction_MapTable, CGameObject);

HRESULT CInteraction_MapTable::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_MapTable::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom            = Add_Component<CCollider>();
    m_pInteractionColliderCom = Add_Component<CCollider>();
    m_pPhysXColliderCom       = Add_Component<CPhysXCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pModelCom.lock()->Init_Model("SM_Bookstand_001", "");

    return S_OK;
}

HRESULT CInteraction_MapTable::Start()
{
    __super::Start();

    SetUpColliderDesc();

    if (LEVEL::LEVEL_EDIT == m_CreatedLevel)
        m_pInteractionColliderCom.lock()->Set_Enable(false);

    return S_OK;
}

void CInteraction_MapTable::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_MapTable::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_MapTable::Render(ID3D11DeviceContext* pDeviceContext)
{
    return __super::Render(pDeviceContext);
}

void CInteraction_MapTable::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);
}

void CInteraction_MapTable::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);
}

void CInteraction_MapTable::Act_Interaction()
{
    switch (GET_SINGLE(CGameManager)->Get_PreLevel())
    {
        case LEVEL::LEVEL_STAGE2:
        {
            Weak_Cast<CClientLevel>(GAMEINSTANCE->Get_CurrentLevel()).lock()->ExitLevel(LEVEL::LEVEL_STAGE3);
            GAMEINSTANCE->PlaySound2D("EVM_Fantasy_Game_Book_Page_Turn_1.ogg", 1.f);
        }
        break;

        default:
        {
            Weak_Cast<CClientLevel>(GAMEINSTANCE->Get_CurrentLevel()).lock()->ExitLevel(LEVEL::LEVEL_GAMEPLAY);
            GAMEINSTANCE->PlaySound2D("EVM_Fantasy_Game_Book_Page_Turn_1.ogg", 1.f);
        }
        break;
    }
}

void CInteraction_MapTable::SetUpColliderDesc()
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale = _float3(0.5f, 0.f, 0.f);

    m_pInteractionColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pInteractionColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

    MESH_VTX_INFO tInfo = m_pModelCom.lock()->Get_ModelData().lock()->VertexInfo;

    XMStoreFloat3(&m_vCenterOffset, (XMLoadFloat3(&tInfo.vMin) + XMLoadFloat3(&tInfo.vMax)) / 2.f);
    tInfo.vCenter = m_vCenterOffset;
    m_fCullingOffsetRange = XMVectorGetX(XMVector3Length(XMVectorAbs(XMLoadFloat3(&tInfo.vMax)) + XMVectorAbs(XMLoadFloat3(&tInfo.vMin))));

    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::INTERIOR;
    ColliderDesc.vScale       = _float3((tInfo.vMax.x - tInfo.vMin.x), (tInfo.vMax.y - tInfo.vMin.y), (tInfo.vMax.z - tInfo.vMin.z));
    ColliderDesc.vRotation    = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, tInfo.vCenter.y, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::OBB, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

    PHYSXCOLLIDERDESC tDesc;
    Preset::PhysXColliderDesc::StaticInteriorBoxDefaultSetting(tDesc, m_pTransformCom, tInfo, &m_fPhyxOffset);
    m_pPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
    m_pPhysXColliderCom.lock()->Add_PhysXActorAtScene();
    m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom, XMVectorSet(0.f, m_fPhyxOffset, 0.f, 0.f));
}

void CInteraction_MapTable::OnDestroy()
{
}

void CInteraction_MapTable::Free()
{
}
