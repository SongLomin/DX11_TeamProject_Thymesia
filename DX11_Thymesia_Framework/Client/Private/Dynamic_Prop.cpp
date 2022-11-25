#include "stdafx.h"
#include "Dynamic_Prop.h"
#include "Client_Components.h"
#include "Client_Presets.h"

GAMECLASS_C(CDynamic_Prop);
CLONE_C(CDynamic_Prop, CGameObject);

HRESULT CDynamic_Prop::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CDynamic_Prop::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom = Add_Component<CCollider>();
    m_pModelCom.lock()->Init_Model("DistructableTest");
    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pPhysXColliderCom = Add_Component<CPhysXCollider>();

    m_pPhysXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), true);

   
    

    return S_OK;
}

HRESULT CDynamic_Prop::Start()
{
    COLLIDERDESC tColliderDesc;
    tColliderDesc.iLayer = (_uint)COLLISION_LAYER::DYNAMIC_PROP;
    tColliderDesc.vRotation = { 0.f, 0.f, 0.f, 0.f };
    tColliderDesc.vScale = { 2.f, 0.f, 0.f };
    XMStoreFloat3(&tColliderDesc.vTranslation, m_pTransformCom.lock()->Get_Position());

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, tColliderDesc);

    PhysXColliderDesc tPhysXColliderDesc;
    Preset::PhysXColliderDesc::StaticPropSetting(tPhysXColliderDesc, m_pTransformCom);
    m_pPhysXColliderCom.lock()->CreatePhysXActor(tPhysXColliderDesc);
    m_pPhysXColliderCom.lock()->Add_PhysXActorAtScene();

    return S_OK;
}

void CDynamic_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CDynamic_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CDynamic_Prop::Render()
{
    __super::Render();

    return S_OK;
}

void CDynamic_Prop::Free()
{
}
