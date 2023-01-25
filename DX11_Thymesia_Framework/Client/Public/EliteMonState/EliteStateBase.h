
#pragma once
#include "AIStateBase.h"

BEGIN(Engine)
class CAnimation;
class CNavigation;
END

BEGIN(Client)

class CMonster;
class CEliteStateBase abstract :
    public CAIStateBase
{
    GAMECLASS_H(CEliteStateBase);

public:
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void    Start() override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;

public:
    virtual _bool Check_RequirementCoolDown(weak_ptr<CEliteStateBase> pTargetState, const _float& In_fCoolTime);
    virtual void Init_Desc(void* In_pDesc) override;

protected:
    void            Set_AtkPermitAndNoHurt(_bool AtkPermitAndNoHurt) { m_bAtkPermitAndNoHurt = AtkPermitAndNoHurt; }
    void            Turn_ToThePlayer(_float fTimeDelta);
    _vector         Get_InputToLookDir();
    void            StartPositonLookAt(_float fTimeDelta);
    _bool           Rotation_InputToLookDir();
    _bool           Rotation_TargetToLookDir();
    _float          Get_DistanceWithPlayer() const;


protected:
    _uint                m_iAtkCounterGauge = 0;
    weak_ptr<CAnimation> m_pThisAnimationCom;

protected:
    virtual void TurnMechanism() override;
    void TurnAttack(_float fTimeDelta);

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