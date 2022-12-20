#pragma once
#include "BossBat/BatBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBatBossState_Stun_Loop :
	public CBatBossStateBase
{
	GAMECLASS_H(CBatBossState_Stun_Loop);
	CLONE_H(CBatBossState_Stun_Loop, CComponent)
		SHALLOW_COPY(CBatBossState_Stun_Loop)



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


protected:

	void Free();

};

END






