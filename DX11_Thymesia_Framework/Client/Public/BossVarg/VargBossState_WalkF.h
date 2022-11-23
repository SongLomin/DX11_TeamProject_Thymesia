#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_WalkF :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_WalkF);
	CLONE_H(CVargBossState_WalkF, CComponent)
		SHALLOW_COPY(CVargBossState_WalkF)



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
	_float m_fCurrentSpeed = 1.5f;
	_float m_fAccel = 0.f;



private:
	//void Call_AnimationEnd();
protected:
	//virtual void OnDestroy() override;
	void Free();

};

END






