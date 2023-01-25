#pragma once
#include "PlayerStateBase.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)
class CCorvusStateBase abstract :
    public CPlayerStateBase
{
    GAMECLASS_H(CCorvusStateBase)

public:
	virtual void Call_OtherControllerHit(const PxControllersHit& In_hit) override;
	virtual void OnEventMessage(_uint iArg) override;
	void Free();

protected:
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;

protected:
    virtual _bool Check_RequirementAttackState();
    virtual _bool Check_RequirementDashState();
    virtual _bool Check_RequirementJoggingState();
    virtual _bool Check_RequirementSprintState();
    virtual _bool Check_RequirementRunState();
    virtual _bool Check_RequirementAVoidState();
    virtual _bool Check_RequirementParryState();
    virtual _bool Check_RequirementUltimateState();
    virtual _bool Check_RequirementHealingState();
    virtual _bool Check_RequirementClawAttackState();
    virtual _bool Check_RequirementClawAttackHoldState();
    virtual _bool Check_RequirementObjectInteraction();
    virtual _bool Check_RequirementUpLadderState();
    virtual _bool Check_RequirementDownLadderState();
    virtual _bool Check_RequirementFeatherAttackState();
    virtual _bool Check_RequirementExcuteState(weak_ptr<CGameObject>& Out_pGameObject);
    virtual  void Check_AndChangeHitState(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;
    virtual _int  Check_AndChangeSuccessParrying(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider);
    virtual _bool Check_AndChangeLadderState(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider);
    virtual _bool Check_RequirementAttackClose(weak_ptr<CGameObject>& Out_pGameObject);

private:
    _float m_fTimeAcc = 0.f;

private:
    FDelegate<_float> CallBack_WaterWave;

private:
    void Add_WaveDesc(_float In_fTimeDelta);

protected:
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;

    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

    virtual void HitEffectSound();
};

END
