#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_RaidAttack :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_RaidAttack);
	CLONE_H(CVargBossState_RaidAttack, CComponent)
		SHALLOW_COPY(CVargBossState_RaidAttack)

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
	DECAL_DESC m_DecalDesc;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
};

END






