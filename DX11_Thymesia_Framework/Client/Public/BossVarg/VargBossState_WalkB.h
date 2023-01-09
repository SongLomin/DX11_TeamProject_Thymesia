#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_WalkB :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_WalkB);
	CLONE_H(CVargBossState_WalkB, CComponent)
		SHALLOW_COPY(CVargBossState_WalkB)



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
	_float m_fMaxSpeed = 4.f;
	_float m_fCurrentSpeed = 2.5f;
	_float m_fAccel = 0.f;
	_bool  m_bFirstLookAt = true;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);

protected:
	virtual void OnDestroy() override;
	void Free();

};

END






