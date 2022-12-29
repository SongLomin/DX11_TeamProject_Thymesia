#include "stdafx.h"
#include "Interaction_Note.h"

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
#include "imgui.h"

GAMECLASS_C(CInteraction_Note);
CLONE_C(CInteraction_Note, CGameObject);

HRESULT CInteraction_Note::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_Note::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom = Add_Component<CCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    return S_OK;
}

HRESULT CInteraction_Note::Start()
{
    return __super::Start();
}

void CInteraction_Note::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_Note::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_Note::Render(ID3D11DeviceContext* pDeviceContext)
{
    return __super::Render(pDeviceContext);
}
void CInteraction_Note::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_EDITINIT:
        {
            _float fDefaultDesc[4] = { 1.f, 0.f, 0.f, 0.f };
            SetUpColliderDesc(fDefaultDesc);
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
        }
        break;
    }
}


void CInteraction_Note::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

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

void CInteraction_Note::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    if (In_Json.end() != In_Json.find("ColliderDesc"))
    {
        _float fColliderDesc[4];

        for (_uint i = 0; i < 4; ++i)
            fColliderDesc[i] = In_Json["ColliderDesc"][i];

        SetUpColliderDesc(fColliderDesc);
    }
    else
    {
        _float fDefaultDesc[4] = { 1.f, 0.f, 0.f, 0.f };

        SetUpColliderDesc(fDefaultDesc);
    }
}

void CInteraction_Note::Act_Interaction()
{

}

void CInteraction_Note::SetUpColliderDesc(_float* _pColliderDesc)
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale       = _float3(_pColliderDesc[0], 0.f, 0.f);
    ColliderDesc.vTranslation = _float3(_pColliderDesc[1], _pColliderDesc[2], _pColliderDesc[3]);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CInteraction_Note::Free()
{
}