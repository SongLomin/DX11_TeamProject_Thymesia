#pragma once
#include "Skill_Interface.h"

BEGIN(Client)

class CPlayerStateBase;

class CSkill_Base : public CSkill_Interface
{

public:
    GAMECLASS_H(CSkill_Base)
    CLONE_H(CSkill_Base, CComponent)
public:
    // CClientComponent��(��) ���� ��ӵ�
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void    Start() override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;
public:
    /*
     ������ �Ҹ��ϴ� ��ų�� �ƴ϶� ü���� �Ҹ��ϴ� ��ų�� �ִٸ�
      override�ؼ� ó���������.
    */
    virtual _bool   Is_UseAble() override;
    virtual void    UseSkill() override;

    _float  Get_RatioCoolDown();

    virtual void    Reset_Skill();
public:
    FDelegate<>             Callback_StartSkill;//��ų�� �����Ҷ� �ߵ�
    FDelegate<_float>       Callback_UpdateCoolDown;//��Ÿ���� ���� �� ƽ���� �ߵ�
    FDelegate<>             Callback_EndCoolDown;//��Ÿ���� �� ������ �� �ߵ�

protected:
    virtual void            Init_SkillInfo()   override;
    virtual void            Init_State()       override;//��ų�� ��!��!��! ���¸� �����ϰ� �־�� �Ѵ�.
   
protected:
    virtual void            Start_Skill() override;
    virtual void            End_Skill() override;

private:
    void                        Free();
};

END
