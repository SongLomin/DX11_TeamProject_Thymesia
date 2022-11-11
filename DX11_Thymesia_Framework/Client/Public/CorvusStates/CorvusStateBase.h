#pragma once
#include "PlayerStateBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CCorvusStateBase abstract :
    public CPlayerStateBase
{
    GAMECLASS_H(CCorvusStateBase)

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


protected:
    virtual void OnHit(weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;

    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider) override;

public:
    virtual void OnEventMessage(_uint iArg) override;
    void Free();

private:
    
};

END