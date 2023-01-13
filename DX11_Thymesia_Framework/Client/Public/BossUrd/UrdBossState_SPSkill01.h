#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUrdBossState_SPSkill01 :
	public CUrdBossStateBase
{
	GAMECLASS_H(CUrdBossState_SPSkill01);
	CLONE_H(CUrdBossState_SPSkill01, CComponent);
	SHALLOW_COPY(CUrdBossState_SPSkill01);

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

	_uint   m_iResetWeaponNum = 0;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
	void Call_NextKeyFrame(const _uint& In_KeyIndex);

protected:
	virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;
	virtual void OnDestroy() override;
	void Free();

private:
	weak_ptr<CAnimation> m_pThisAnimationCom;

};

END






