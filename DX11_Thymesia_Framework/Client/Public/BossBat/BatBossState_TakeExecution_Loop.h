#pragma once
#include "BossBat/BatBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBatBossState_TakeExecution_Loop :
	public CBatBossStateBase
{
	GAMECLASS_H(CBatBossState_TakeExecution_Loop);
	CLONE_H(CBatBossState_TakeExecution_Loop, CComponent)
		SHALLOW_COPY(CBatBossState_TakeExecution_Loop)



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
	_float      m_fDissolveTime = 0.f;

protected:

	void Free();

};

END






