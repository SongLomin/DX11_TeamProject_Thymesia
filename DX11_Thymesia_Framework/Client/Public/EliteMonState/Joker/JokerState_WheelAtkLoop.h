#pragma once
#include "JokerStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CJokerState_WheelAtkLoop :
	public CJokerStateBase
{
	GAMECLASS_H(CJokerState_WheelAtkLoop);
	CLONE_H(CJokerState_WheelAtkLoop, CComponent)
		SHALLOW_COPY(CJokerState_WheelAtkLoop)

public:
	void Set_TurnCheck(_bool TurnCheck) { m_bTurnCheck = TurnCheck; }
	void Set_BackReset(_bool BackReset) { m_bBackReset = BackReset; }


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
	_uint    m_iCount = 0;

private:
	void Call_AnimationEnd();
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






