#pragma once
#include "EliteMonState/BigHandMan/BigHandManStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBigHandManState_Idle :
	public CBigHandManStateBase
{
	GAMECLASS_H(CBigHandManState_Idle);
	CLONE_H(CBigHandManState_Idle, CComponent)
		SHALLOW_COPY(CBigHandManState_Idle)



public:
	void Set_TurnCheck(_bool TurnCheck) { m_bTurnCheck = TurnCheck; }

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
	_bool    m_bComboAttackRooting = false;
	_bool    m_bFightStart = false;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






