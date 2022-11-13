#pragma once
#include "StateBase.h"

BEGIN(Engine)
class CAnimation;
class CNavigation;
END

BEGIN(Client)

class CMonster;
class CAIStateBase abstract:
    public CStateBase
{
    GAMECLASS_H(CAIStateBase);

public:
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void    Start() override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;

public:
    virtual _bool Check_RequirementCoolDown(weak_ptr<CAIStateBase> pTargetState, const _float& In_fCoolTime);

public:
    _float                  Get_StateTimeAcc() const { return m_fStateTimeAcc; }
    shared_ptr<CMonster>    Get_OwnerMonster() const noexcept;
    virtual _bool Check_Requirement() override;

protected:
    void            Turn_ToThePlayer(_float fTimeDelta);
    _vector         Get_InputToLookDir();
    _bool           Rotation_InputToLookDir();
    _bool           Rotation_TargetToLookDir();
    _float          Get_DistanceWithPlayer() const;

protected:
    _float3             m_vLookAtDir;
    _float              m_fCurrentRotateTime = 0.f;
    _float              m_fStateTimeAcc = 100.f;
    NORMONSTERTYPE      m_eNorMonType =     NORMONSTERTYPE::NMON_END;
    NORMONSTERIDLETYPE  m_eNorMonIdleType = NORMONSTERIDLETYPE::IDLEEND;

   



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