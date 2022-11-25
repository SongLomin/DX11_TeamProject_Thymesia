#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUrdBossState_Idle :
	public CUrdBossStateBase
{
	GAMECLASS_H(CUrdBossState_Idle);
	CLONE_H(CUrdBossState_Idle, CComponent)
		SHALLOW_COPY(CUrdBossState_Idle)



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
	_bool    m_bNextState = false;
	_float   m_fSinematic = 0.f;

private:
//	void Call_AnimationEnd();
protected:
	//virtual void OnDestroy() override;
	void Free();

};

END






