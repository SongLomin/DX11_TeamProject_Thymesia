#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUrdBossState_Attack02LV2C1 :
	public CUrdBossStateBase
{
	GAMECLASS_H(CUrdBossState_Attack02LV2C1);
	CLONE_H(CUrdBossState_Attack02LV2C1, CComponent)
		SHALLOW_COPY(CUrdBossState_Attack02LV2C1)



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
	void Call_AnimationEnd();
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






