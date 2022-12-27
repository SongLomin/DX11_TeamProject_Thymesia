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

public:
	void Set_bPhase2(_bool iPhase2) { m_iPhase2 = iPhase2; }

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
	_uint  m_iPhase2 = 0;


private:
	_bool      m_bOneCheck = false;

private:
	virtual void OnEventMessage(_uint iArg) override;
protected:
	//virtual void OnDestroy() override;
	void Free();

};

END






