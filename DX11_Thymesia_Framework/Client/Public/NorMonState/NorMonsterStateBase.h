#pragma once
#include "AIStateBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CNorMonsterStateBase abstract :
    public CAIStateBase
{
    GAMECLASS_H(CNorMonsterStateBase)

protected:
    virtual _bool Check_RequirementAttackState();
    virtual _bool Check_RequirementDashState();
    virtual _bool Check_RequirementRunState();
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
     void Free();

protected:
    weak_ptr<CAnimation> m_pThisAnimationCom;
};

END
