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

weak_ptr<CSkill_Base> CPlayerSkill_System::Get_MainSkill()
{
    return m_pSkillList[(_uint)SOCKET_TYPE::SOCKET_MAIN];
}


void CPlayerSkill_System::OnChangeSkill(weak_ptr<CSkill_Base> pSkill, SOCKET_TYPE eType)
{
    pSkill.lock()->Reset_Skill();
    m_pSkillList[(_uint)eType] = pSkill;


   // Callback_OnChangeSkill[(_uint)eType].Clear();//스킬의 바인딩은 무조건 하나만 되어있어야한다. 
    //기존 (기존에도 하나겠지만) 바인딩되었던 함수를 제거하고 콜백을 호출해야함.
    //이러니까 내꺼 날려서 잠시 대기


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
