#pragma once
#include "JokerStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CJokerState_WalkB :
	public CJokerStateBase
{
	GAMECLASS_H(CJokerState_WalkB);
	CLONE_H(CJokerState_WalkB, CComponent)
		SHALLOW_COPY(CJokerState_WalkB)

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

private:

	_float m_fMaxSpeed = 3.f;
	_float m_fCurrentSpeed = 1.2f;
	_float m_fAccel = 0.f;


	//TODO 가라에요 ㅎ
	_bool  m_bPlayerToClose = false;


protected:
	void Free();

};

END






