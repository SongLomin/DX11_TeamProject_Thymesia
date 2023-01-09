#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_Attack3a :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_Attack3a);
	CLONE_H(CVargBossState_Attack3a, CComponent)
		SHALLOW_COPY(CVargBossState_Attack3a)

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

	virtual void OnDestroy() override;
	void Free();

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
};

END






