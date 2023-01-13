#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_SPA_Roar_Getup :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_SPA_Roar_Getup);
	CLONE_H(CVargBossState_SPA_Roar_Getup, CComponent)
		SHALLOW_COPY(CVargBossState_SPA_Roar_Getup)

public:
	void Call_NextKeyFrame(const _uint& In_KeyIndex);

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
	_float m_fStopAnimationTimeAcc = 0.f;

	_bool m_bLightEvent = true;
	_bool m_bTrailEvent = true;
	_bool m_bStopAnimation = false;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();
};

END






