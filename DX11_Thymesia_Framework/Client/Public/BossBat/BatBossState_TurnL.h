#pragma once
#include "BossBat/BatBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBatBossState_TurnL :
	public CBatBossStateBase
{
	GAMECLASS_H(CBatBossState_TurnL);
	CLONE_H(CBatBossState_TurnL, CComponent)
		SHALLOW_COPY(CBatBossState_TurnL)



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
	_bool  m_bTurnCheck = false;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






