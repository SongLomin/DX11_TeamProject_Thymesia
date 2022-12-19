#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_Run :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_Run);
	CLONE_H(CVargBossState_Run, CComponent)
		SHALLOW_COPY(CVargBossState_Run)

public:
	void Call_NextKeyFrame(const _uint& In_KeyIndex);

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

	virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
	virtual void OnStateEnd() override;
	virtual _bool Check_AndChangeNextState() override;

	void Free();

private:
	_float m_fMaxSpeed = 12.f;
	_float m_fCurrentSpeed = 8.f;
	_float m_fAccel = 0.f;
};

END






