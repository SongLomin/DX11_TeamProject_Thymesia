#pragma once
#include "BossBat/BatBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBatBossState_Atk_L01_2b :
	public CBatBossStateBase
{
	GAMECLASS_H(CBatBossState_Atk_L01_2b);
	CLONE_H(CBatBossState_Atk_L01_2b, CComponent)
		SHALLOW_COPY(CBatBossState_Atk_L01_2b)



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
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






