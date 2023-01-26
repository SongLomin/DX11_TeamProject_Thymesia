#include "stdafx.h"
#include "Interaction_Elevator.h"

#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "Model.h"

#include "GameInstance.h"
#include "ClientLevel.h"
#include "imgui.h"

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

    m_eInteractionType = INTERACTION_ELEVATOR;

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

HRESULT CInteraction_Elevator::Render(ID3D11DeviceContext* pDeviceContext)
{
    return __super::Render(pDeviceContext);
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

        case EVENT_TYPE::ON_EDIT_UDATE:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

            if (ImGui::InputFloat("Offset Y", &m_fColliderPosY))
            {
                COLLIDERDESC ColliderDesc;
                ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

                ColliderDesc = m_pColliderCom.lock()->Get_ColliderDesc();
                ColliderDesc.vTranslation.y = m_fColliderPosY;

                m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
                m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
            }

            static const char* item_LevelTag[] =
            {
                "LEVEL_STATIC",
                "LEVEL_LOADING",
                "LEVEL_LOGO",
                "LEVEL_LOBBY",
                "LEVEL_GAMEPLAY",
                "LEVEL_STAGE1",
                "LEVEL_STAGE2",
                "LEVEL_STAGE3",
                "LEVEL_STAGE4",
                "LEVEL_EDIT",
                "LEVEL_TEST",
            };

            _int iSelect_Level = (_int)m_eNextLevel;

            if (ImGui::Combo("Level", &iSelect_Level, item_LevelTag, IM_ARRAYSIZE(item_LevelTag)))
                m_eNextLevel = (LEVEL)iSelect_Level;
        }
        break;
    }
}

void CInteraction_Elevator::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["ColliderPosY"] = m_fColliderPosY;
    Out_Json["NextLevel"]    = (_int)m_eNextLevel;

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);
}

void CInteraction_Elevator::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    if (In_Json.end() != In_Json.find("ColliderPosY"))
        m_fColliderPosY = In_Json["ColliderPosY"];

    if (In_Json.end() != In_Json.find("NextLevel"))
    {
        _int iLevel = In_Json["NextLevel"];
        m_eNextLevel = (LEVEL)iLevel;
    }
    
    SetUpColliderDesc();
}

void CInteraction_Elevator::Act_Interaction()
{
    if (LEVEL::LEVEL_END == m_eNextLevel)
        return;

    Weak_Cast<CClientLevel>(GAMEINSTANCE->Get_CurrentLevel()).lock()->ExitLevel(m_eNextLevel);
    GAMEINSTANCE->PlaySound2D("EVM_Fantasy_Game_Door_Close.ogg", 1.f);
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

void CInteraction_Elevator::Free()
{
}
