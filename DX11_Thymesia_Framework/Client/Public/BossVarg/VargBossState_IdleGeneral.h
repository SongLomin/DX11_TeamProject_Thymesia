#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_IdleGeneral :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_IdleGeneral);
	CLONE_H(CVargBossState_IdleGeneral, CComponent)
		SHALLOW_COPY(CVargBossState_IdleGeneral)



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






