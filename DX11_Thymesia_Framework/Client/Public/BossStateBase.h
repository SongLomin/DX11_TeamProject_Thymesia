
#pragma once
#include "AIStateBase.h"

BEGIN(Engine)
class CAnimation;
class CNavigation;
END

BEGIN(Client)

class CMonster;
class CBossStateBase abstract :
    public CAIStateBase
{
    GAMECLASS_H(CBossStateBase);

public:
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void    Start() override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;

public:
    virtual _bool Check_RequirementCoolDown(weak_ptr<CBossStateBase> pTargetState, const _float& In_fCoolTime);
    virtual void Init_Desc(void* In_pDesc) override;

public:
    _float                  Get_StateTimeAcc() const { return m_fStateTimeAcc; }
    shared_ptr<CMonster>    Get_OwnerMonster() const noexcept;
    virtual _bool Check_Requirement() override;

protected:
    void            Set_AtkPermitAndNoHurt(_bool AtkPermitAndNoHurt) { m_bAtkPermitAndNoHurt = AtkPermitAndNoHurt; }
    void            Turn_ToThePlayer(_float fTimeDelta);
    _vector         Get_InputToLookDir();
    void            StartPositonLookAt(_float fTimeDelta);
    _bool           Rotation_InputToLookDir();
    _bool           Rotation_TargetToLookDir();
    _bool           Rotation_TargetToLookRevirseDir();
    _float          Get_DistanceWithPlayer() const;


protected:
   _uint                m_iAtkCounterGauge = 0;

protected:
    _vector Get_CurMonToStartMonDir();
    _float GetStartPositionToCurrentPositionDir();
    void TurnMechanism();
    void TurnAttack(_float fTimeDelta);
    _bool JumpLookOffsetLookAt();
    _float ComputeAngleWithPlayer();
    _int ComputeDirectionToPlayer();

private:
    weak_ptr<CMonster> m_pOwnerFromMonster;

public: /* For. EventFunction */
    virtual void OnDestroy() override;
    virtual void OnEnable(void* _Arg = nullptr) override;
    virtual void OnDisable() override;

    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

protected:
    void Free();

};

END