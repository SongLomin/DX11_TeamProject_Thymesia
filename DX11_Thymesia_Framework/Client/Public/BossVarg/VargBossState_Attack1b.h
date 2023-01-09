#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_Attack1b :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_Attack1b);
	CLONE_H(CVargBossState_Attack1b, CComponent)
		SHALLOW_COPY(CVargBossState_Attack1b)

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

private:
	_bool  m_bNextAttack = false;
};

END






