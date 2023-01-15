#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_Exe_Start :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_Exe_Start);
	CLONE_H(CVargBossState_Exe_Start, CComponent)
		SHALLOW_COPY(CVargBossState_Exe_Start)

public:
	void  Set_DieType(_bool DieType) { m_bDieType = DieType; }

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
	virtual void Call_NextKeyFrame(const _uint& In_iKeyIndex) override;

private:
	_bool   m_bDieType = false;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
	virtual void OnEventMessage(_uint iArg) override;
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






