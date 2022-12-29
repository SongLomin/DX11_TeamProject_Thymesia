#include "stdafx.h"
#include "PlayerSkill_System.h"
#include "ClientComponent_Utils.h"
#include "Skill_Base.h"

GAMECLASS_C(CPlayerSkill_System);
CLONE_C(CPlayerSkill_System, CComponent)


HRESULT CPlayerSkill_System::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerSkill_System::Initialize(void* pArg)
{

    return S_OK;
}

void CPlayerSkill_System::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    
    Tick_SkillList(fTimeDelta);

}

void CPlayerSkill_System::LateTick(_float fTimeDelta)
{

}

void CPlayerSkill_System::Start()
{
    __super::Start();



}

void CPlayerSkill_System::ResetAllSkillCoolDown()
{
    for (_uint i = 0; i < (_uint)SOCKET_TYPE::SOCKET_END; i++)
    {
        if (m_pSkillList[i].lock())
        {
            m_pSkillList[i].lock()->Reset_Skill();
        }
    }
}

void CPlayerSkill_System::UseMainSKill()
{
    if (!m_pSkillList[(_uint)SOCKET_TYPE::SOCKET_MAIN].lock())
        return;

    m_pSkillList[(_uint)SOCKET_TYPE::SOCKET_MAIN].lock()->UseSkill();
}

void CPlayerSkill_System::SwapSkillMaintoSub()
{
    //서브나 메인 스킬 둘중에 하나라도 없으면 스왑 X
    if (!m_pSkillList[(_uint)SOCKET_TYPE::SOCKET_MAIN].lock() || !m_pSkillList[(_uint)SOCKET_TYPE::SOCKET_SUB].lock())
        return;

    weak_ptr<CSkill_Base>   pSkillBase_Main = m_pSkillList[(_uint)SOCKET_TYPE::SOCKET_MAIN];
    weak_ptr<CSkill_Base>   pSkillBase_Sub = m_pSkillList[(_uint)SOCKET_TYPE::SOCKET_SUB];

    UnBindSkill(SOCKET_TYPE::SOCKET_MAIN);
    UnBindSkill(SOCKET_TYPE::SOCKET_SUB);

    OnChangeSkill(pSkillBase_Sub, SOCKET_TYPE::SOCKET_MAIN);
    OnChangeSkill(pSkillBase_Main, SOCKET_TYPE::SOCKET_SUB);
}


void CPlayerSkill_System::UnBindSkill(SOCKET_TYPE eType)
{
    if (!m_pSkillList[(_uint)eType].lock())
        return;

    m_pSkillList[(_uint)eType].lock()->Clear_Callback();
    m_pSkillList[(_uint)eType] = weak_ptr<CSkill_Base>();
}

void CPlayerSkill_System::OnChangeSkill(weak_ptr<CSkill_Base> pSkill, SOCKET_TYPE eType)
{
    if (m_pSkillList[(_uint)eType].lock())
    {
        m_pSkillList[(_uint)eType].lock()->Clear_Callback();
    }
    if (!pSkill.lock())
    {
        DEBUG_ASSERT;
    }
    
    m_pSkillList[(_uint)eType] = pSkill;
   
    Callback_OnChangeSkill[(_uint)eType](pSkill);
}
void CPlayerSkill_System::Tick_SkillList(_float fTimeDelta)
{
    for (_uint i = 0; i < (_uint)SOCKET_TYPE::SOCKET_END; i++)
    {
        if (m_pSkillList[i].lock())
        {
            m_pSkillList[i].lock()->Tick(fTimeDelta);
        }
    }
}
