#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
class CAnimation;
END

BEGIN(Client)

class CVargBossState_Exe_Dead :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_Exe_Dead);
	CLONE_H(CVargBossState_Exe_Dead, CComponent)
		SHALLOW_COPY(CVargBossState_Exe_Dead)



protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

protected:
	virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;
	virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
	virtual void OnStateEnd() override;
	virtual _bool Check_AndChangeNextState() override;


public:
	void Call_NextKeyFrame(const _uint& In_iKeyIndex);

private:
	weak_ptr<CAnimation> m_ThisStateAnimationCom;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






