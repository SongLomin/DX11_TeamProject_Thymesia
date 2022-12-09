#include "stdafx.h"
#include "Section_Eventer.h"

#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Collider.h"

#include "GameInstance.h"

GAMECLASS_C(CSection_Eventer);
CLONE_C(CSection_Eventer, CGameObject);

HRESULT CSection_Eventer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSection_Eventer::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pShaderCom        = Add_Component<CShader>();
    m_pRendererCom      = Add_Component<CRenderer>();
    m_pColliderCom      = Add_Component<CCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    return S_OK;
}

HRESULT CSection_Eventer::Start()
{
    return __super::Start();
}

void CSection_Eventer::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CSection_Eventer::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSection_Eventer::Render()
{
    return __super::Render();
}

void CSection_Eventer::OnEventMessage(_uint iArg)
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

void CSection_Eventer::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);
}

void CSection_Eventer::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);
}

void CSection_Eventer::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CSection_Eventer::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CSection_Eventer::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CSection_Eventer::SetUpColliderDesc(_float* _pColliderDesc)
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale       = _float3(_pColliderDesc[0], 0.f, 0.f);
    ColliderDesc.vTranslation = _float3(_pColliderDesc[1], _pColliderDesc[2], _pColliderDesc[3]);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}
