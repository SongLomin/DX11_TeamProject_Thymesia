#include "stdafx.h"
#include "PlayerSkill_System.h"
#include "ClientComponent_Utils.h"
#include "Skill_Base.h"
#include "PlayerSkillHeader.h"
#include "RequirementChecker.h"
#include "Requirement_Time.h"

GAMECLASS_C(CPlayerSkill_System);
CLONE_C(CPlayerSkill_System, CComponent)


HRESULT CPlayerSkill_System::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerSkill_System::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    USE_START(CPlayerSkill_System);

    return S_OK;
}

void CPlayerSkill_System::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);


    Tick_SkillList(fTimeDelta);
    m_pStealSkill.lock()->Tick(fTimeDelta);


}

void CPlayerSkill_System::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

void CPlayerSkill_System::Start()
{
    __super::Start();


    m_pStealSkill = m_pOwner.lock()->Get_Component<CStolenSkill>();

    SetUp_SkillMapFromMonsterType();
}

void CPlayerSkill_System::ResetAllSkillCoolDown()
{
    for (_uint i = 0; i < (_uint)SOCKET_TYPE::SOCKET_END; i++)
    {
        if (m_SkillList[i].lock())
        {
            m_SkillList[i].lock()->Reset_Skill();
        }
    }
}

void CPlayerSkill_System::UseMainSKill()
{
    if (!m_SkillList[(_uint)SOCKET_TYPE::SOCKET_MAIN].lock())
        return;

    m_SkillList[(_uint)SOCKET_TYPE::SOCKET_MAIN].lock()->UseSkill();
}

void CPlayerSkill_System::UseStealSKill()
{
    if (!m_pStealSkill.lock())
        return;

   
    m_pStealSkill.lock()->UseSkill();
}

void CPlayerSkill_System::SwapSkillMaintoSub()
{
    //서브나 메인 스킬 둘중에 하나라도 없으면 스왑 X
    if (!m_SkillList[(_uint)SOCKET_TYPE::SOCKET_MAIN].lock() || !m_SkillList[(_uint)SOCKET_TYPE::SOCKET_SUB].lock())
        return;

    weak_ptr<CSkill_Base>   pSkillBase_Main = m_SkillList[(_uint)SOCKET_TYPE::SOCKET_MAIN];
    weak_ptr<CSkill_Base>   pSkillBase_Sub = m_SkillList[(_uint)SOCKET_TYPE::SOCKET_SUB];

    UnBindSkill(SOCKET_TYPE::SOCKET_MAIN);
    UnBindSkill(SOCKET_TYPE::SOCKET_SUB);

    OnChangeSkill(pSkillBase_Sub, SOCKET_TYPE::SOCKET_MAIN);
    OnChangeSkill(pSkillBase_Main, SOCKET_TYPE::SOCKET_SUB);
}

HRESULT CPlayerSkill_System::OnStealMonsterSkill(MONSTERTYPE In_eMonsterType)
{
    //몬스터 스킬을 뺏었을 때.
    MONSTERSKILLMAP::iterator mapIter;

    mapIter = m_MonsterSkillMap.find(In_eMonsterType);

    if (mapIter == m_MonsterSkillMap.end())
    {
        //만약에 바인딩된 스킬이 없을 경우, 
        return E_FAIL;
    }
    m_pStealSkill.lock()->OnStealSkill(mapIter->second);
    
    Callback_OnStealSkill(m_pStealSkill);
    return S_OK;
}

void CPlayerSkill_System::UnBindSkill(SOCKET_TYPE eType)
{
    if (!m_SkillList[(_uint)eType].lock())
        return;

    m_SkillList[(_uint)eType].lock()->Clear_Callback();
    m_SkillList[(_uint)eType] = weak_ptr<CSkill_Base>();
}

void CPlayerSkill_System::OnChangeSkill(weak_ptr<CSkill_Base> pSkill, SOCKET_TYPE eType)
{
    if (m_SkillList[(_uint)eType].lock())
    {
        m_SkillList[(_uint)eType].lock()->Clear_Callback();
    }
    if (!pSkill.lock())
    {
        DEBUG_ASSERT;
    }
    
    m_SkillList[(_uint)eType] = pSkill;
   
    Callback_OnChangeSkill[(_uint)eType](pSkill);
}
void CPlayerSkill_System::OnChangeSkill(SKILL_NAME eSkillName, SOCKET_TYPE eType)
{
    SKILLNAMEMAP::iterator iter;

    iter = m_SkillNameMap.find(eSkillName);

    if (iter == m_SkillNameMap.end())
        return;

    OnChangeSkill(iter->second, eType);
}
void CPlayerSkill_System::RegisterSkill(SKILL_NAME eName, weak_ptr<CSkill_Base> pSkill)
{
    m_SkillNameMap.emplace(eName, pSkill);
}
void CPlayerSkill_System::SetUp_SkillMapFromMonsterType()
{
   m_MonsterSkillMap.emplace(MONSTERTYPE::AXEMAN, m_pOwner.lock()->Get_Component<CSkill_Axe>());
   m_MonsterSkillMap.emplace(MONSTERTYPE::SHIELDAXEMAN, m_pOwner.lock()->Get_Component<CSkill_Axe>());
   m_MonsterSkillMap.emplace(MONSTERTYPE::VARG, m_pOwner.lock()->Get_Component<CSkill_VargSword>());
   m_MonsterSkillMap.emplace(MONSTERTYPE::KNIFEWOMAN, m_pOwner.lock()->Get_Component<CSkill_Knife>());
   m_MonsterSkillMap.emplace(MONSTERTYPE::JOKER, m_pOwner.lock()->Get_Component<CSkill_Hammer>());
   m_MonsterSkillMap.emplace(MONSTERTYPE::GARDENER, m_pOwner.lock()->Get_Component<CSkill_Scythe>());
   m_MonsterSkillMap.emplace(MONSTERTYPE::ENHANCE_GARDENER, m_pOwner.lock()->Get_Component<CSkill_Scythe>());
   m_MonsterSkillMap.emplace(MONSTERTYPE::BAT, m_pOwner.lock()->Get_Component<CSkill_BloodStorm>());

}
void CPlayerSkill_System::Tick_SkillList(_float fTimeDelta)
{
    for (_uint i = 0; i < (_uint)SOCKET_TYPE::SOCKET_END; i++)
    {
        if (m_SkillList[i].lock())
        {
            m_SkillList[i].lock()->Tick(fTimeDelta);
        }
    }
}

void CPlayerSkill_System::Free()
{
    int a = 0;
}
