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
    m_pModelCom.lock()->Init_Model("Wagon3_Origin");
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
    Preset::PhysXColliderDesc::DynamicPropSetting(tPhysXColliderDesc, m_pTransformCom);
    m_pPhysXColliderCom.lock()->CreatePhysXActor(tPhysXColliderDesc);
    m_pPhysXColliderCom.lock()->Add_PhysXActorAtScene();

    return S_OK;
}

void CDynamic_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

#ifdef _LIFEGUARD_FOR_FALL_
    _vector vPos = m_pTransformCom.lock()->Get_Position();
    if (vPos.m128_f32[1] < -50.f)
    {
        vPos.m128_f32[1] = 50.f;
        m_pTransformCom.lock()->Set_Position(vPos);
    }
#endif
}

void CDynamic_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);
}

void CDynamic_Prop::Before_Render(_float fTimeDelta)
{
    __super::Before_Render(fTimeDelta);

    m_pPhysXColliderCom.lock()->Synchronize_Transform(m_pTransformCom);

}

HRESULT CDynamic_Prop::Render()
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
            m_iPassIndex = 3;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex);
        m_pModelCom.lock()->Render_Mesh(i);
    }

    return S_OK;
}

void CDynamic_Prop::Free()
{
}
