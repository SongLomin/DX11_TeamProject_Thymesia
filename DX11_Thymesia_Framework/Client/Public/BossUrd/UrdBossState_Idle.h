#pragma once
#include "BossUrd/UrdBossState_IdleStates.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUrdBossState_Idle :
	public CUrdBossState_IdleStates
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
	virtual void OnEventMessage(_uint iArg) override;
private:
	//TODO юс╫ц
	_int        m_iCount = 0;


protected:
	void Free();

};

END






