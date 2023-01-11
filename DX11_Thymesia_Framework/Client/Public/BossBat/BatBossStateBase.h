#pragma once
#include "BossStateBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CBoneNode;
class CBoneNode;
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
    virtual _int  Check_DotAttackState();
    virtual _bool Check_RequirementPlayerInRange(const _float& In_fRange);

protected:
    void Play_OnHitEffect();

    _matrix Get_LeftHandCombinedWorldMatrix();
    _matrix Get_RightHandCombinedWorldMatrix();

protected:
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;

    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

protected:
    _bool                m_bRootStop = true;
    _bool                m_bOne = true; // 0�̸� ���� 1�̸� ��ž
    _bool                m_bTurnAttack = false;

    weak_ptr<CBoneNode> m_pLeftHandBoneNode;
    weak_ptr<CBoneNode> m_pRightHandBoneNode;
public:
    virtual void OnEventMessage(_uint iArg) override;
    void Free();



};

END
