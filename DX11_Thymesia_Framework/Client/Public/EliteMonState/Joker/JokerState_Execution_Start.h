#pragma once
#include "JokerStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CJokerState_TakeExecution_Start :
	public CJokerStateBase
{
	GAMECLASS_H(CJokerState_TakeExecution_Start);
	CLONE_H(CJokerState_TakeExecution_Start, CComponent)
		SHALLOW_COPY(CJokerState_TakeExecution_Start)




protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

protected:
	virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
	virtual void OnStateEnd() override;
	virtual _bool Check_AndChangeNextState() override;
	virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;
private:
	_bool   m_bOnce = false;

	//TODO 
	_bool m_bTwoOnce = false;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






