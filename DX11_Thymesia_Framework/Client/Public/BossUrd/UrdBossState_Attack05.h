#pragma once
#include "BossUrd/UrdBossState_Attacks.h"

BEGIN(Client)
class CUrdBossState_Attack05 final :
	public CUrdBossState_Attacks
{
	GAMECLASS_H(CUrdBossState_Attack05);
	CLONE_H(CUrdBossState_Attack05, CComponent);
	SHALLOW_COPY(CUrdBossState_Attack05);

public:
	virtual void Call_AnimationEnd(_uint iEndAnimIndex) override;
	virtual void Call_NextKeyFrame(const _uint& In_KeyIndex) override;

private:
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
};

END






