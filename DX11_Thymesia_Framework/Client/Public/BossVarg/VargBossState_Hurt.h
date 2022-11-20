#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_Hurt :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_Hurt);
	CLONE_H(CVargBossState_Hurt, CComponent)
		SHALLOW_COPY(CVargBossState_Hurt)



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
	//void Call_AnimationEnd();
protected:
	//virtual void OnDestroy() override;
	void Free();

};

END






