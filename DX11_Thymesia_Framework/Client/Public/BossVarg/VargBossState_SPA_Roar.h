#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_SPA_Roar :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_SPA_Roar);
	CLONE_H(CVargBossState_SPA_Roar, CComponent)
		SHALLOW_COPY(CVargBossState_SPA_Roar)


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
	weak_ptr<CAnimation> m_pThisAnimationCom;
	_bool	m_bShakingCamera = false;

private:
	void Call_AnimationEnd();
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






