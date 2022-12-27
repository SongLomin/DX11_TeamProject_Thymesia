#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_Idle :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_Idle);
	CLONE_H(CVargBossState_Idle, CComponent)
		SHALLOW_COPY(CVargBossState_Idle)

public:
	void Set_TurnCheck(_bool TurnCheck) { m_bTurnCheck = TurnCheck; }
	void Set_BackReset(_bool BackReset) { m_bBackReset = BackReset; }
	void Set_BackCount(_uint BackCount) { m_iBackCount = BackCount; }

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
	_bool    m_bTurnCheck = false;
	_bool    m_bBackReset = false;
	_uint    m_iBackCount = 0;

private:
	virtual void OnEventMessage(_uint iArg) override;
protected:
	//virtual void OnDestroy() override;
	void Free();

};

END






