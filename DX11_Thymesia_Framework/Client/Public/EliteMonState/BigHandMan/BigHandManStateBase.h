#pragma once
#include "EliteMonState/EliteStateBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CBigHandManStateBase abstract :
    public CEliteStateBase
{
    GAMECLASS_H(CBigHandManStateBase)

public:
	virtual void OnEventMessage(_uint iArg) override;
	virtual void OnEventMessage(weak_ptr<CBase> pArg)override;

protected:
    virtual HRESULT Initialize(void* pArg) override;
    virtual _bool Check_RequirementAttackState();
    virtual _bool Check_RequirementDashState();
    virtual _bool Check_RequirementRunState();
    virtual _uint Check_RequirementDotState();
    virtual _bool Check_RequirementPlayerInRange(const _float& In_fRange);

    void Play_OnHitEffect();

    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;

    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    void Free();
};

END
