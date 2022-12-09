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

    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));
    m_eInteractionType = INTERACTION_DOOR;

    return S_OK;
}

HRESULT CInteraction_Door::Start()
{
    return __super::Start();
}

void CInteraction_Door::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_Door::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);
}

HRESULT CInteraction_Door::Render()
{
    return __super::Render();
}

void CInteraction_Door::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_EDITINIT:
        {
            _float fDefaultDesc[4] = { 3.f, 0.f, 1.5f, 0.f };
            SetUpColliderDesc(fDefaultDesc);
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

            SetUp_Invisibility();

            ImGui::DragFloat("Rotation Speed ", &m_fRotationtSpeed);
            ImGui::DragFloat("Rotation Radian", &m_fRotationtRadian);

            COLLIDERDESC ColliderDesc;
            ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

            ColliderDesc = m_pColliderCom.lock()->Get_ColliderDesc();

            _bool bChage = false;

            bChage |= ImGui::DragFloat3("Coll Transform", &ColliderDesc.vTranslation.x);
            bChage |= ImGui::DragFloat ("Coll Size"     , &ColliderDesc.vScale.x);

            if (bChage)
            {
                m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
                m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
            }
        }
        break;
    }
}

void CInteraction_Door::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["RotationtRadian"]   = m_fRotationtRadian;
    Out_Json["RotationSpeed"]     = m_fRotationtSpeed;

    if (m_pColliderCom.lock())
    {
        COLLIDERDESC ColliderDesc;
        ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

        ColliderDesc = m_pColliderCom.lock()->Get_ColliderDesc();

        _float ColliderInfo[4] = 
        {
            ColliderDesc.vScale.x,
            ColliderDesc.vTranslation.x,
            ColliderDesc.vTranslation.y,
            ColliderDesc.vTranslation.z
        };

        Out_Json["ColliderDesc"] = ColliderInfo;
    }
}

void CInteraction_Door::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    if (In_Json.end() != In_Json.find("RotationtRadian"))
        m_fRotationtRadian = In_Json["RotationtRadian"];

    if (In_Json.end() != In_Json.find("RotationSpeed"))
        m_fRotationtSpeed = In_Json["RotationSpeed"];

    if (In_Json.end() != In_Json.find("ColliderDesc"))
    {
        _float fColliderDesc[4];

        for (_uint i = 0; i < 4; ++i)
            fColliderDesc[i] = In_Json["ColliderDesc"][i];

        SetUpColliderDesc(fColliderDesc);
    }
    else
    {
        _float fDefaultDesc[4] = { 3.f, 0.f, 1.5f, 0.f };

        SetUpColliderDesc(fDefaultDesc);
    }

    m_fFirstRadian = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(m_pTransformCom.lock()->Get_WorldMatrix())).y;

    if ("" == string(m_pModelCom.lock()->Get_ModelKey()))
        m_pModelCom.lock()->Init_Model("Door01_05", "");

    m_pPhysXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), true);
    PhysXColliderDesc tDesc;
    Preset::PhysXColliderDesc::ConvexStaticPropSetting(tDesc, m_pTransformCom);
    m_pPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
    m_pPhysXColliderCom.lock()->Add_PhysXActorAtSceneWithOption();
}

void CInteraction_Door::Act_OpenDoor(_float fTimeDelta, _bool& Out_IsEnd)
{
    m_fAddRadian += fTimeDelta * m_fRotationtSpeed;

    _float fDir = (0.f == m_fRotationtSpeed) ? (1.f) : (m_fRotationtSpeed / abs(m_fRotationtSpeed));
    m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), ((m_fFirstRadian + (m_fAddRadian))));

    if (m_fRotationtRadian <= fabs(m_fAddRadian))
    {
        m_ActionFlag ^= ACTION_FLAG::ROTATION;
        Out_IsEnd     = true;
        m_fAddRadian  = 0.f;

        Callback_ActEnd();
    }
}

void CInteraction_Door::Act_CloseDoor(_float fTimeDelta, _bool& Out_IsEnd)
{
    m_fAddRadian += fTimeDelta * m_fRotationtSpeed;

    _float fDir = (0.f == m_fRotationtSpeed) ? (1.f) : (m_fRotationtSpeed / abs(m_fRotationtSpeed));
    m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), ((m_fFirstRadian + (m_fRotationtRadian * fDir)) - (m_fAddRadian)));

    if (m_fRotationtRadian <= fabs(m_fAddRadian))
    {
        m_ActionFlag ^= ACTION_FLAG::ROTATION;
        Out_IsEnd     = true;
        m_fAddRadian  = 0.f;

        Callback_ActEnd();
    }
}

void CInteraction_Door::Act_Interaction()
{
    if (m_ActionFlag & ACTION_FLAG::ROTATION)
        Callback_ActUpdate += bind(&CInteraction_Door::Act_OpenDoor, this, placeholders::_1, placeholders::_2);
    else
        Callback_ActUpdate += bind(&CInteraction_Door::Act_CloseDoor, this, placeholders::_1, placeholders::_2);
}

void CInteraction_Door::SetUpColliderDesc(_float* _pColliderDesc)
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale       = _float3(_pColliderDesc[0], 0.f, 0.f);
    ColliderDesc.vTranslation = _float3(_pColliderDesc[1], _pColliderDesc[2], _pColliderDesc[3]);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}
