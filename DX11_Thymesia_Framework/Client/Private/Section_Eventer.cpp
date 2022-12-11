#include "stdafx.h"
#include "Section_Eventer.h"

#include "Transform.h"
#include "Collider.h"

#include "GameInstance.h"
#include "GameManager.h"

GAMECLASS_C(CSection_Eventer);
CLONE_C(CSection_Eventer, CGameObject);

HRESULT CSection_Eventer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSection_Eventer::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom = Add_Component<CCollider>();

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
            _float fDefaultDesc[4] = { 6.f, 0.f, 0.f, 0.f };
            SetUpColliderDesc(fDefaultDesc);
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
            
            ImGui::Text("");
            ImGui::Separator();


            COLLIDERDESC ColliderDesc;
            ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

            ColliderDesc = m_pColliderCom.lock()->Get_ColliderDesc();

            _bool bChage = false;

            bChage |= ImGui::InputFloat("Coll Size", &ColliderDesc.vScale.x);

            if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
            {
                ColliderDesc.vScale.x += 0.1f;
                bChage = true;
            }
            else if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
            {
                ColliderDesc.vScale.x -= 0.1f;
                bChage = true;
            }

            if (bChage)
            {
                m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
                m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
            }

            ImGui::InputInt("Section", &m_iSectionIndex);

            _bool bCheckState[] =
            {
                (m_Flag & EVENT_FLAG::ACT_SECTION),
                (m_Flag & EVENT_FLAG::ACT_MONSTER_TRIGGER),

                (m_Flag & EVENT_FLAG::EVENT_ENTER),
                (m_Flag & EVENT_FLAG::EVENT_STAY),
                (m_Flag & EVENT_FLAG::EVENT_EXIT)
            };

            if (ImGui::Checkbox("ACT_SECTION", &bCheckState[0]))
                m_Flag ^= EVENT_FLAG::ACT_SECTION;

            if (ImGui::Checkbox("ACT_MONSTER_TRIGGER", &bCheckState[1]))
                m_Flag ^= EVENT_FLAG::ACT_MONSTER_TRIGGER;

            if (ImGui::Checkbox("EVENT_ENTER", &bCheckState[2]))
                m_Flag ^= EVENT_FLAG::EVENT_ENTER;

            if (ImGui::Checkbox("EVENT_STAY", &bCheckState[3]))
                m_Flag ^= EVENT_FLAG::EVENT_STAY;

            if (ImGui::Checkbox("EVENT_Exit", &bCheckState[4]))
                m_Flag ^= EVENT_FLAG::EVENT_EXIT;
        }
        break;
    }
}

void CSection_Eventer::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["Flag"]         = m_Flag;
    Out_Json["ColliderSize"] = m_pColliderCom.lock()->Get_ColliderDesc().vScale.x;
    Out_Json["SectionIndex"] = m_iSectionIndex;
}

void CSection_Eventer::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    m_Flag          = In_Json["Flag"];
    m_iSectionIndex = In_Json["SectionIndex"];

    _float fColliderSize   = In_Json["ColliderSize"];
    _float fDefaultDesc[4] = { fColliderSize, 0.f, 0.f, 0.f };
    SetUpColliderDesc(fDefaultDesc);
}

void CSection_Eventer::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    if (!(m_Flag & EVENT_FLAG::EVENT_ENTER) && LEVEL::LEVEL_EDIT == m_CreatedLevel)
        return;

    __super::OnCollisionEnter(pMyCollider, pOtherCollider);

    if (m_Flag & EVENT_FLAG::ACT_SECTION)
    {
        GET_SINGLE(CGameManager)->Activate_Section(m_iSectionIndex, true);
        m_pColliderCom.lock()->Set_Enable(false);
    }
}

void CSection_Eventer::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    if (!(m_Flag & EVENT_FLAG::EVENT_STAY) && LEVEL::LEVEL_EDIT == m_CreatedLevel)
        return;

    __super::OnCollisionStay(pMyCollider, pOtherCollider);

    if (m_Flag & EVENT_FLAG::ACT_SECTION)
    {
        GET_SINGLE(CGameManager)->Activate_Section(m_iSectionIndex, true);
        m_pColliderCom.lock()->Set_Enable(false);
    }
}

void CSection_Eventer::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    if (!(m_Flag & EVENT_FLAG::EVENT_EXIT) && LEVEL::LEVEL_EDIT == m_CreatedLevel)
        return;

    __super::OnCollisionExit(pMyCollider, pOtherCollider);

    if (m_Flag & EVENT_FLAG::ACT_SECTION)
    {
        GET_SINGLE(CGameManager)->Activate_Section(m_iSectionIndex, true);
        m_pColliderCom.lock()->Set_Enable(false);
    }
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
