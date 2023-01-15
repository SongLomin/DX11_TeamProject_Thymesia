#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Engine)
class CModel;
class CAnimation;
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

private:
	//TODO: Test For Color Inversion
	void Calculate_Inversion(_float In_fTimeDelta, _bool& In_bEnd);

	_uint   m_iResetWeaponNum = 0;
private:
	FDelegate<_float, _bool&> CallBack_ColorInversion;

	_float m_fInversionStrength = 1.f;
	_float m_fInversionRatio = 0.f;

	_bool m_bCameraShaking = false;
	_float m_fShakingRatio = 0.01f;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
	void Call_NextKeyFrame(const _uint& In_KeyIndex);

private:
	DECAL_DESC m_DecalDesc;
	weak_ptr<CAnimation> m_pThisAnimationCom;

protected:
	virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;
	virtual void OnDestroy() override;
	void Free();


};

END






