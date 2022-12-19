#pragma once
#include "JokerStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CJokerState_ComboB2 :
	public CJokerStateBase
{
	GAMECLASS_H(CJokerState_ComboB2);
	CLONE_H(CJokerState_ComboB2, CComponent)
	SHALLOW_COPY(CJokerState_ComboB2)

public:
	void Call_NextKeyFrame(const _uint& In_KeyIndex);
	void Set_TurnCheck(_bool TurnCheck) { m_bTurnCheck = TurnCheck; }
	void Set_BackReset(_bool BackReset) { m_bBackReset = BackReset; }

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
	void Call_AnimationEnd();

private:
	_bool    m_bTurnCheck = false;
	_bool    m_bBackReset = false;
};

END






