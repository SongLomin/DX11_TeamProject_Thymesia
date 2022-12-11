#pragma once
#include "EliteMonState/EliteStateBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CJokerStateBase abstract :
    public CEliteStateBase
{
    GAMECLASS_H(CJokerStateBase)

protected:
    virtual _bool Check_RequirementAttackState();
    virtual _bool Check_RequirementDashState();
    virtual _bool Check_RequirementRunState();
    virtual _uint Check_RequirementDotState();
    virtual _bool Check_RequirementPlayerInRange(const _float& In_fRange);

protected:
    void Play_OnHitEffect();

protected:
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;

    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

public:
    virtual void OnEventMessage(_uint iArg) override;
    virtual void OnEventMessage(weak_ptr<CBase> pArg)override;
    void Free();



};

END
