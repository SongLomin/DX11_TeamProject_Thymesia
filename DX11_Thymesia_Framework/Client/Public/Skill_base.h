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
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void    Start() override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;
public:
    /*
     마나를 소모하는 스킬이 아니라 체력을 소모하는 스킬이 있다면
      override해서 처리해줘야함.
    */
    virtual _bool   Is_UseAble() override;
    virtual void    UseSkill() override;

    _float  Get_RatioCoolDown();

    virtual void    Reset_Skill();
public:
    FDelegate<>             Callback_StartSkill;//스킬이 시작할때 발동
    FDelegate<_float>       Callback_UpdateCoolDown;//쿨타임이 도는 매 틱마다 발동
    FDelegate<>             Callback_EndCoolDown;//쿨타임이 다 돌았을 때 발동

protected:
    virtual void            Init_SkillInfo()   override;
    virtual void            Init_State()       override;//스킬은 무!조!건! 상태를 참조하고 있어야 한다.
   
protected:
    virtual void            Start_Skill() override;
    virtual void            End_Skill() override;

private:
    void                        Free();
};

END
