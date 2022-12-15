#pragma once
#include "ClientComponent.h"

BEGIN(Client)

class CRequirementChecker;
class CRequirement_Time;
class CRequirement_PlayerStatusMana;

class CSkill_Interface : public CClientComponent
{
public:
    GAMECLASS_H(CSkill_Interface)


public:
    _float      Get_SkillCollDown() { return m_fSkillCoolDown; }

    SKILL_NAME  Get_SkillName()     { return m_eSkillName; }
    SKILL_TYPE  Get_SkillType()     { return m_eSkillType; }



    /*
     마나를 소모하는 스킬이 아니라 체력을 소모하는 스킬이 있다면
      override해서 처리해줘야함.
    */
    virtual _bool   Is_UseAble()    PURE;
    virtual void    UseSkill()      PURE;

protected:
    virtual void            Init_SkillInfo()   PURE;
    virtual void            Init_State()       PURE;//스킬은 무!조!건! 상태를 참조하고 있어야 한다.


protected:
    virtual void            Start_Skill() PURE;
    virtual void            End_Skill() PURE;

protected:
    shared_ptr<CRequirementChecker>     m_pRequirementChecker;
    
    shared_ptr<CRequirement_Time>               m_pRequirementTime;
    shared_ptr<CRequirement_PlayerStatusMana>   m_pRequirementMana;


    _bool                               m_bUseAble;
    SKILL_NAME                          m_eSkillName;
    SKILL_TYPE                          m_eSkillType;
    _float                              m_fSkillCoolDown;
    
    _float                              m_fRequiredCost;
    weak_ptr<CPlayerStateBase>          m_pSkillState;

};

END