#include "stdafx.h"
#include "Section_Eventer.h"

#include "Transform.h"
#include "Collider.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "imgui.h"

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

#ifdef _DEBUG
    m_pRendererCom = Add_Component<CRenderer>();
    m_pColliderCom.lock()->Set_DebugColor(XMVectorSet(1.f, 0.f, 0.f, 1.f));
#endif

    return S_OK;
}

HRESULT CSection_Eventer::Start()
{
    if (0 <= m_iSectionIndex)
        GET_SINGLE(CGameManager)->Registration_Section(m_iSectionIndex, Weak_Cast<CSection_Eventer>(m_this));

    return __super::Start();
}

void CSection_Eventer::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CSection_Eventer::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

#ifdef _DEBUG
    m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONLIGHT, Weak_StaticCast<CGameObject>(m_this));
#endif
}

HRESULT CSection_Eventer::Render(ID3D11DeviceContext* pDeviceContext)
{

#ifdef _DEBUG
    m_pColliderCom.lock()->Render_IgnoreDebugCheck();
#endif

    return __super::Render(pDeviceContext);
}

void CSection_Eventer::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_RESET_OBJ:
        {
            m_pColliderCom.lock()->Set_Enable(true);
        }
        break;

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

            _bool bCheckState_Event[] =
            {
                (_bool)(m_EventFlag & EVENT_FLAG::EVENT_ENTER),
                (_bool)(m_EventFlag & EVENT_FLAG::EVENT_STAY),
                (_bool)(m_EventFlag & EVENT_FLAG::EVENT_EXIT)
            };

            _bool bCheckState_Act[] =
            {
                (_bool)(m_ActFlag & ACT_FLAG::ACT_SECTION),
                (_bool)(m_ActFlag & ACT_FLAG::ACT_MONSTER_TRIGGER),
                (_bool)(m_ActFlag & ACT_FLAG::ACT_LIGHT),
                (_bool)(m_ActFlag & ACT_FLAG::ACT_FOG),
            };

            if (ImGui::Checkbox("EVENT_ENTER", &bCheckState_Event[0]))
                m_EventFlag ^= EVENT_FLAG::EVENT_ENTER;

            if (ImGui::Checkbox("EVENT_STAY", &bCheckState_Event[1]))
                m_EventFlag ^= EVENT_FLAG::EVENT_STAY;

            if (ImGui::Checkbox("EVENT_Exit", &bCheckState_Event[2]))
                m_EventFlag ^= EVENT_FLAG::EVENT_EXIT;


            if (ImGui::Checkbox("ACT_SECTION", &bCheckState_Act[0]))
                m_ActFlag ^= ACT_FLAG::ACT_SECTION;

            if (ImGui::Checkbox("ACT_MONSTER_TRIGGER", &bCheckState_Act[1]))
                m_ActFlag ^= ACT_FLAG::ACT_MONSTER_TRIGGER;

            if (ImGui::Checkbox("ACT_LIGHT", &bCheckState_Act[2]))
                m_ActFlag ^= ACT_FLAG::ACT_LIGHT;

            if (ImGui::Checkbox("ACT_FOG", &bCheckState_Act[3]))
                m_ActFlag ^= ACT_FLAG::ACT_FOG;
        }
        break;
    }
}

void CSection_Eventer::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["EventFlag"]    = m_EventFlag;
    Out_Json["ActFlag"]      = m_ActFlag;
    Out_Json["ColliderSize"] = m_pColliderCom.lock()->Get_ColliderDesc().vScale.x;
    Out_Json["SectionIndex"] = m_iSectionIndex;
}

void CSection_Eventer::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    m_EventFlag     = In_Json["EventFlag"];
    m_ActFlag       = In_Json["ActFlag"];
    m_iSectionIndex = In_Json["SectionIndex"];

    _float fColliderSize   = In_Json["ColliderSize"];
    _float fDefaultDesc[4] = { fColliderSize, 0.f, 0.f, 0.f };
    SetUpColliderDesc(fDefaultDesc);
}

void CSection_Eventer::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    if (!(m_EventFlag & EVENT_FLAG::EVENT_ENTER) || LEVEL::LEVEL_EDIT == m_CreatedLevel)
        return;

    __super::OnCollisionEnter(pMyCollider, pOtherCollider);

    if (m_ActFlag & ACT_FLAG::ACT_SECTION)
    {
        GET_SINGLE(CGameManager)->Activate_Section(m_iSectionIndex, EVENT_TYPE::ON_ENTER_SECTION);
        m_pColliderCom.lock()->Set_Enable(false);
        Set_Enable(false);
    }

    if (m_ActFlag & ACT_FLAG::ACT_LIGHT)
    {
        GET_SINGLE(CGameManager)->Activate_SectionLight(m_iSectionIndex, EVENT_TYPE::ON_ENTER_SECTION);
        m_pColliderCom.lock()->Set_Enable(false);
        Set_Enable(false);
    }

    if (m_ActFlag & ACT_FLAG::ACT_FOG)
    {
        GET_SINGLE(CGameManager)->Activate_Fog(m_iSectionIndex);
        m_pColliderCom.lock()->Set_Enable(false);
        Set_Enable(false);
    }
}

void CSection_Eventer::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    if (!(m_EventFlag & EVENT_FLAG::EVENT_STAY) || LEVEL::LEVEL_EDIT == m_CreatedLevel)
        return;

    __super::OnCollisionStay(pMyCollider, pOtherCollider);

    if (m_ActFlag & ACT_FLAG::ACT_SECTION)
    {
        GET_SINGLE(CGameManager)->Activate_Section(m_iSectionIndex, EVENT_TYPE::ON_ENTER_SECTION);
        m_pColliderCom.lock()->Set_Enable(false);
        Set_Enable(false);
    }

    if (m_ActFlag & ACT_FLAG::ACT_LIGHT)
    {
        GET_SINGLE(CGameManager)->Activate_SectionLight(m_iSectionIndex, EVENT_TYPE::ON_ENTER_SECTION);
        m_pColliderCom.lock()->Set_Enable(false);
        Set_Enable(false);
    }

    if (m_ActFlag & ACT_FLAG::ACT_FOG)
    {
        GET_SINGLE(CGameManager)->Activate_Fog(m_iSectionIndex);
        m_pColliderCom.lock()->Set_Enable(false);
        Set_Enable(false);
    }
}

void CSection_Eventer::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    if (!(m_EventFlag & EVENT_FLAG::EVENT_EXIT) || LEVEL::LEVEL_EDIT == m_CreatedLevel)
        return;

    __super::OnCollisionExit(pMyCollider, pOtherCollider);

    if (m_ActFlag & ACT_FLAG::ACT_SECTION)
    {
        GET_SINGLE(CGameManager)->Activate_Section(m_iSectionIndex, EVENT_TYPE::ON_ENTER_SECTION);
        m_pColliderCom.lock()->Set_Enable(false);
    }

    if (m_ActFlag & ACT_FLAG::ACT_LIGHT)
    {
        GET_SINGLE(CGameManager)->Activate_SectionLight(m_iSectionIndex, EVENT_TYPE::ON_ENTER_SECTION);
        m_pColliderCom.lock()->Set_Enable(false);
    }

    if (m_ActFlag & ACT_FLAG::ACT_FOG)
    {
        GET_SINGLE(CGameManager)->Activate_Fog(m_iSectionIndex);
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

void CSection_Eventer::OnDestroy()
{
    __super::OnDestroy();

    GET_SINGLE(CGameManager)->Remove_Section(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));
}

void CSection_Eventer::Free()
{
}
