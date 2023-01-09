#pragma once
#include "NorMonsterStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_SpecialAttackLoop :
	public CNorMonsterStateBase
{
	GAMECLASS_H(CNorMonState_SpecialAttackLoop);
	CLONE_H(CNorMonState_SpecialAttackLoop, CComponent)
		SHALLOW_COPY(CNorMonState_SpecialAttackLoop)

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
	_float m_fMaxSpeed = 12.f;
	_float m_fCurrentSpeed =9.6f;
	_float m_fAccel = 0.f;

private:
	//void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	//virtual void OnDestroy() override;
	void Free();

};

END






