#include "stdafx.h"
#include "Interaction_Fence.h"

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

GAMECLASS_C(CInteraction_Fence);
CLONE_C(CInteraction_Fence, CGameObject);

HRESULT CInteraction_Fence::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_Fence::Initialize(void* pArg)
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

HRESULT CInteraction_Fence::Start()
{
    __super::Start();

    XMStoreFloat4(&m_vFirstPosition, m_pTransformCom.lock()->Get_Position());

    m_pPhysXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), false);
    PhysXColliderDesc tDesc;
    Preset::PhysXColliderDesc::StaticPropSetting(tDesc, m_pTransformCom);
    m_pPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
    m_pPhysXColliderCom.lock()->Add_PhysXActorAtSceneWithOption();

    return S_OK;
}

void CInteraction_Fence::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_Fence::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);
}

HRESULT CInteraction_Fence::Render(ID3D11DeviceContext* pDeviceContext)
{
    return __super::Render(pDeviceContext);
}

void CInteraction_Fence::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_ENTER_SECTION:
        {
            if (!Callback_ActUpdate.empty())
                return;

            if (!m_ActionFlag)
                return;

            Callback_ActUpdate += bind(&CInteraction_Fence::Act_OpenDoor, this, placeholders::_1, placeholders::_2);
        }
        break;

        case EVENT_TYPE::ON_EXIT_SECTION:
        {
            if (!Callback_ActUpdate.empty())
                return;

            if (m_ActionFlag)
                return;

            // Close
            Callback_ActUpdate += bind(&CInteraction_Fence::Act_CloseDoor, this, placeholders::_1, placeholders::_2);
        }
        break;

        case EVENT_TYPE::ON_LOCK_SECTION:
        {
            if (m_ActionFlag)
                return;

            m_pColliderCom.lock()->Set_Enable(false);
        }
        break;

        case EVENT_TYPE::ON_RESET_OBJ:
        {
            m_pTransformCom.lock()->Set_Position(XMLoadFloat4(&m_vFirstPosition));
            m_pPhysXColliderCom.lock()->Set_Enable(true);

            m_ActionFlag = true;
            Callback_ActUpdate.Clear();
        }
        break;

        case EVENT_TYPE::ON_UNLOCK_SECTION:
        {
            m_pColliderCom.lock()->Set_Enable(true);
        }
        break;

        case EVENT_TYPE::ON_EDITINIT:
        {
            if (0.f == m_pColliderCom.lock()->Get_ColliderDesc().vScale.x)
            {
                _float fDefaultDesc[4] = { 3.f, 0.f, 1.5f, 0.f };
                SetUpColliderDesc(m_pColliderCom, fDefaultDesc, COLLISION_LAYER::TRIGGER);
            }

            if (!m_pDirColliderCom.lock())
            {
                m_pDirColliderCom = Add_Component<CCollider>();

                _float fDefaultDesc[4] = { 1.f, 0.f, 1.5f, 1.f };
                SetUpColliderDesc(m_pDirColliderCom, fDefaultDesc, COLLISION_LAYER::CHECK_DIR);
                m_pDirColliderCom.lock()->Set_Enable(false);
            }
        }
        break;

        case EVENT_TYPE::ON_EDIT_UDATE:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
            m_pDirColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            SetUp_Invisibility();

            ImGui::DragFloat("Move Distance", &m_fMoveDistance);
            ImGui::DragFloat("Move Speed"   , &m_fMovetSpeed);      
        }
        break;
    }
}

void CInteraction_Fence::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["MoveDistance"]   = m_fMoveDistance;
    Out_Json["MovetSpeed"]     = m_fMovetSpeed;
    Out_Json["SectionIndex"]   = m_iSectionIndex;

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

void CInteraction_Fence::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    m_fMoveDistance = In_Json["MoveDistance"];
    m_fMovetSpeed   = In_Json["MovetSpeed"];
    m_iSectionIndex = In_Json["SectionIndex"];

    if (0 <= m_iSectionIndex)
        GET_SINGLE(CGameManager)->Registration_Section(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));

    if (In_Json.end() != In_Json.find("ColliderDesc"))
    {
        _float fColliderDesc[4];

        for (_uint i = 0; i < 4; ++i)
            fColliderDesc[i] = In_Json["ColliderDesc"][i];

        SetUpColliderDesc(m_pColliderCom, fColliderDesc, COLLISION_LAYER::TRIGGER);
    }
    else
    {
        _float fDefaultDesc[4] = { 3.f, 0.f, 1.5f, 0.f };
        SetUpColliderDesc(m_pColliderCom, fDefaultDesc, COLLISION_LAYER::TRIGGER);
    }
}

void CInteraction_Fence::Act_OpenDoor(_float fTimeDelta, _bool& Out_IsEnd)
{
    m_fAddDistance += fTimeDelta * m_fMovetSpeed;

    _vector vPos = XMLoadFloat4(&m_vFirstPosition) + XMVectorSet(0.f, m_fAddDistance, 0.f, 0.f);
    m_pTransformCom.lock()->Set_Position(vPos);

    if (m_fAddDistance <= fabs(m_fMoveDistance))
    {
        m_ActionFlag   = false;
        Out_IsEnd      = true;
        m_fAddDistance = 0.f;

        Callback_ActEnd();
    }
}

void CInteraction_Fence::Act_CloseDoor(_float fTimeDelta, _bool& Out_IsEnd)
{
    m_fAddDistance += fTimeDelta * m_fMovetSpeed;

    _vector vPos = XMLoadFloat4(&m_vFirstPosition) + XMVectorSet(0.f,  m_fAddDistance - m_fMoveDistance, 0.f, 0.f);
    m_pTransformCom.lock()->Set_Position(vPos);

    if(m_fAddDistance <= fabs(m_fMoveDistance))
    {
        m_ActionFlag   = true;
        Out_IsEnd      = true;
        m_fAddDistance = 0.f;

        Callback_ActEnd();
    }
}

void CInteraction_Fence::Act_Interaction()
{
    if (m_ActionFlag)
    {
        Callback_ActUpdate += bind(&CInteraction_Fence::Act_OpenDoor, this, placeholders::_1, placeholders::_2);
        m_pPhysXColliderCom.lock()->Set_Enable(false);
    }
    else
    {
        Callback_ActUpdate += bind(&CInteraction_Fence::Act_CloseDoor, this, placeholders::_1, placeholders::_2);
    }
}

void CInteraction_Fence::SetUpColliderDesc(weak_ptr<CCollider> In_pColldierCom, _float* _pColliderDesc, COLLISION_LAYER _eCollLayer)
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)_eCollLayer;
    ColliderDesc.vScale       = _float3(_pColliderDesc[0], 0.f, 0.f); 
    ColliderDesc.vTranslation = _float3(_pColliderDesc[1], _pColliderDesc[2], _pColliderDesc[3]);

    In_pColldierCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    In_pColldierCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CInteraction_Fence::OnDestroy()
{
    __super::OnDestroy();

    GET_SINGLE(CGameManager)->Remove_SectionLight(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));
}

void CInteraction_Fence::Free()
{
}
