#pragma once
#include "JokerStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CJokerState_RunAttackLoop :
	public CJokerStateBase
{
	GAMECLASS_H(CJokerState_RunAttackLoop);
	CLONE_H(CJokerState_RunAttackLoop, CComponent)
	SHALLOW_COPY(CJokerState_RunAttackLoop)

public:
	void Call_NextKeyFrame(const _uint& In_KeyIndex);

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

	virtual void OnDestroy() override;
	void Free();

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);

private:
	_float m_fMaxSpeed = 4.f;
	_float m_fCurrentSpeed = 1.5f;
	_float m_fAccel = 0.f;
};

END






