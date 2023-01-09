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

    m_pColliderCom       = Add_Component<CCollider>();
    
    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pModelCom.lock()->Init_Model("SM_ToolRackArranged", "");

    return S_OK;
}

HRESULT CInteraction_MapTable::Start()
{
    __super::Start();

    if (LEVEL::LEVEL_EDIT == m_CreatedLevel)
        m_pColliderCom.lock()->Set_Enable(false);

    if (LEVEL::LEVEL_EDIT == m_CreatedLevel)
        m_pColliderCom.lock()->Set_Enable(false);

    SetUpColliderDesc();

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

}

void CInteraction_MapTable::SetUpColliderDesc()
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale = _float3(3.f, 0.f, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CInteraction_MapTable::OnDestroy()
{
}

void CInteraction_MapTable::Free()
{
}
