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
#include "GameManager.h"
#include "Status_Player.h"

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

HRESULT CInteraction_CheckPoint::Render(ID3D11DeviceContext* pDeviceContext)
{
    return __super::Render(pDeviceContext);
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

        case EVENT_TYPE::ON_EDIT_UDATE:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
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

            ImGui::InputInt("CheckIndex", &m_iCheckIndex);
        }
        break;
    }
}


void CInteraction_CheckPoint::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);

    Out_Json["CheckIndex"] = m_iCheckIndex;
}

void CInteraction_CheckPoint::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    if (In_Json.end() != In_Json.find("CheckIndex"))
    {
        m_iCheckIndex = In_Json["CheckIndex"];

        // TODO : Save ����� ������ ��� �����ϱ�, �������� ���۽� ó�� �ε��� ���ڸ� üũ����Ʈ�� ����ϴ� �����
        if (0 == m_iCheckIndex)
            GET_SINGLE(CGameManager).get()->Registration_CheckPoint(Weak_Cast<CInteraction_CheckPoint>(m_this));
    }

    SetUpColliderDesc();
}

void CInteraction_CheckPoint::Act_Interaction()
{
    GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front().lock()->Call_Landing(CUI_Landing::LANDING_BECONFOUND);

    weak_ptr<CStatus_Player> pPlayerStatus = GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status();

    if (pPlayerStatus.lock())
        pPlayerStatus.lock()->Full_Recovery();

    GET_SINGLE(CGameManager).get()->Registration_CheckPoint(Weak_Cast<CInteraction_CheckPoint>(m_this));
}

void CInteraction_CheckPoint::SetUpColliderDesc()
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer = (_uint)COLLISION_LAYER::CHECKPOINT;
    ColliderDesc.vScale = _float3(3.f, 0.f, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}
