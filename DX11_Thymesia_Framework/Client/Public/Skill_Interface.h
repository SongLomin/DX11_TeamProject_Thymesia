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
     ������ �Ҹ��ϴ� ��ų�� �ƴ϶� ü���� �Ҹ��ϴ� ��ų�� �ִٸ�
      override�ؼ� ó���������.
    */
    virtual _bool   Is_UseAble()    PURE;
    virtual void    UseSkill()      PURE;

protected:
    virtual void            Init_SkillInfo()   PURE;
    virtual void            Init_State()       PURE;//��ų�� ��!��!��! ���¸� �����ϰ� �־�� �Ѵ�.


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