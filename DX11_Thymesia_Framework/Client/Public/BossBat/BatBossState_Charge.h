#pragma once
#include "BossBat/BatBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBatBossState_Charge :
	public CBatBossStateBase
{
	GAMECLASS_H(CBatBossState_Charge);
	CLONE_H(CBatBossState_Charge, CComponent)
		SHALLOW_COPY(CBatBossState_Charge)

public:
	void Set_HellScream(_uint iHellSceram) { m_iHellSceram = iHellSceram; }
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
	_uint       m_iHellSceram = 0;
private:
	void Call_AnimationEnd();
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






