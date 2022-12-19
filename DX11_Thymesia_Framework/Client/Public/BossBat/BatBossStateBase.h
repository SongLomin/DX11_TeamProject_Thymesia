#pragma once
#include "BossStateBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CBatBossStateBase abstract :
    public CBossStateBase
{
    GAMECLASS_H(CBatBossStateBase)

protected:
    virtual _bool Check_RequirementAttackState();
    virtual _bool Check_RequirementDashState();
    virtual _bool Check_RequirementRunState();
    virtual _bool Check_CrossAttackState();
    virtual _bool Check_CrossJumpState();
    virtual _bool Check_RequirementPlayerInRange(const _float& In_fRange);

protected:
    void Play_OnHitEffect();

protected:
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;

    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

protected:
    _bool                m_bRootStop = true;
    _bool                m_bOne = true; // 0이면 정상 1이면 스탑
    _bool                m_bTurnAttack = false;
public:
    virtual void OnEventMessage(_uint iArg) override;
    void Free();



};

END
