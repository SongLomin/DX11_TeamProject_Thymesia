#include "stdafx.h"
#include "Dynamic_Prop.h"

#include "Client_Components.h"
#include "Client_Presets.h"
#include "Dynamic_Piece.h"
#include "Client_Presets.h"

#include "GameManager.h"

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
    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pPhysXColliderCom = Add_Component<CPhysXCollider>();

    return S_OK;
}

HRESULT CDynamic_Prop::Start()
{
    COLLISION_LAYER CollisionLayer = COLLISION_LAYER::DYNAMIC_PROP;

    if (!m_pModelCom.lock()->Get_ModelData().lock() && (_uint)LEVEL_EDIT != m_CreatedLevel)
    {
        m_pModelCom.lock()->Init_Model("ColumnDamage08_Origin");
        m_pTransformCom.lock()->Set_Scaled({ 0.5f, 0.5f, 0.5f });
        m_pPhysXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), true);
        Preset::Dynamic_Prop::GateWay_DynamicPropSetting(m_pModelCom.lock()->Get_ModelKey(), m_CreatedLevel, m_pPieces);
    }

    if (hash<string>()("ColumnDamage08_Origin") == hash<string>()(m_pModelCom.lock()->Get_ModelKey()))
    {
        CollisionLayer = COLLISION_LAYER::BOSS_DYNAMIC_PROP;
        m_szSoundKey = "Impact_Rock_Pillar_Large_Impact_04.wav";
    }
    else if (hash<string>()("Wagon3_Origin") == hash<string>()(m_pModelCom.lock()->Get_ModelKey()))
    {
        m_szSoundKey = "EVM_prop_hell_wood_crate_bones_break_01.ogg";
    }
    else if (hash<string>()("Fence_16a_Origin") == hash<string>()(m_pModelCom.lock()->Get_ModelKey()))
    {
        m_szSoundKey = "EVM_prop_hell_wood_crate_bones_break_02.ogg";
    }

    __super::Start();

    COLLIDERDESC tColliderDesc;
    tColliderDesc.iLayer       = (_uint)CollisionLayer;
    tColliderDesc.vRotation    = { 0.f, 0.f, 0.f, 0.f };
    tColliderDesc.vScale       = { 2.3f, 0.f, 0.f };
    tColliderDesc.vOffset      = { 0.f, 0.f, 0.f };
    tColliderDesc.vTranslation = { 0.f, 0.f, 0.f };

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, tColliderDesc);

    if ((_uint)LEVEL_EDIT != m_CreatedLevel)
    {
        PhysXColliderDesc tPhysXColliderDesc;
        Preset::PhysXColliderDesc::DynamicPropSetting(tPhysXColliderDesc, m_pTransformCom);
        m_pPhysXColliderCom.lock()->CreatePhysXActor(tPhysXColliderDesc);
        m_pPhysXColliderCom.lock()->Add_PhysXActorAtScene();
    }

    // GET_SINGLE(CGameManager)->Registration_OnlyResetObject(Weak_Cast<CGameObject>(m_this));

    return S_OK;
}

void CDynamic_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

//#ifdef _LIFEGUARD_FOR_FALL_
//    _vector vPos = m_pTransformCom.lock()->Get_Position();
//    if (vPos.m128_f32[1] < -50.f)
//    {
//        vPos.m128_f32[1] = 50.f;
//        m_pTransformCom.lock()->Set_Position(vPos);
//    }
//#endif
}

void CDynamic_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CDynamic_Prop::Before_Render(_float fTimeDelta)
{
    __super::Before_Render(fTimeDelta);

    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

HRESULT CDynamic_Prop::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(CProp::SetUp_ShaderResource()))
        return E_FAIL;

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
            if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
                m_iPassIndex = 6;
            else
                m_iPassIndex = 7;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    CProp::Render(pDeviceContext);

    return S_OK;
}

_bool CDynamic_Prop::IsPicking(const RAY& In_Ray, _float& Out_fRange)
{
    if (!m_pModelCom.lock().get())
        return false;

    return m_pModelCom.lock()->IsModelPicking(In_Ray, Out_fRange);
}

void CDynamic_Prop::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    if ((_uint)LEVEL_EDIT != m_CreatedLevel && m_pModelCom.lock()->Get_ModelData().lock())
    {
        m_pPhysXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), true);
        Preset::Dynamic_Prop::GateWay_DynamicPropSetting(m_pModelCom.lock()->Get_ModelKey(), m_CreatedLevel, m_pPieces);
    }
}

void CDynamic_Prop::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionEnter(pMyCollider, pOtherCollider);

    _matrix WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();

    for (auto& elem : m_pPieces)
    {
        elem.lock()->Set_WorldMatrixWithPhysX(WorldMatrix);
        elem.lock()->Set_Enable(true);
    }

    GAMEINSTANCE->PlaySound3D(m_szSoundKey, 1.f, m_pTransformCom.lock()->Get_Position());
    Set_Dead();
}

void CDynamic_Prop::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_RESET_OBJ:
        {

        }
        break;
    }
}

void CDynamic_Prop::Free()
{
}
