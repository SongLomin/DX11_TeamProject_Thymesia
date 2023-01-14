#pragma once
#include "BossBat/BatBossState_IdleStates.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBatBossState_HellIdle :
	public CBatBossState_IdleStates
{
	GAMECLASS_H(CBatBossState_HellIdle);
	CLONE_H(CBatBossState_HellIdle, CComponent)
		SHALLOW_COPY(CBatBossState_HellIdle)


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
	_int  m_iPreCount = 4;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	virtual void OnEventMessage(_uint iArg) override;
	void Free();

};

END






