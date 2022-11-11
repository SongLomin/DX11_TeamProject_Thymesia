#pragma once
#include "StateBase.h"

BEGIN(Engine)
class CAnimation;
class CNavigation;

END

BEGIN(Client)

class CBehaviorBase;
class CPlayer;

class CPlayerStateBase abstract:
    public CStateBase
{
    GAMECLASS_H(CPlayerStateBase);

public:
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void    Start() override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;

protected:
    shared_ptr<CPlayer> Get_OwnerPlayer() const noexcept;
    virtual _bool Check_Requirement() override;

protected:
    void            Turn_Transform(_float fTimeDelta);
    _vector         Get_InputToLookDir();
    _bool           Rotation_InputToLookDir();
    _bool           Rotation_TargetToLookDir();
    _bool           Rotation_NearToLookDir();

protected:
    _float3                m_vLookAtDir;
    _float                 m_fCurrentRotateTime = 0.f;
    
    weak_ptr<CPlayer>      m_pOwnerFromPlayer;


public: /* For. EventFunction */
    void OnWeaponFirstAttack(weak_ptr<CCollider> pOtherCollider);
    void OnWeaponAttack(weak_ptr<CCollider> pOtherCollider);

    virtual void OnDestroy() override;
    virtual void OnEnable(void* _Arg = nullptr) override;
    virtual void OnDisable() override;

    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

protected:
    void Free();

};

END