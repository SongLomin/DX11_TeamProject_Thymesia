#pragma once
#include "AIStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_Walk_B :
	public CAIStateBase
{
	GAMECLASS_H(CNorMonState_Walk_B);
	CLONE_H(CNorMonState_Walk_B, CComponent)
		SHALLOW_COPY(CNorMonState_Walk_B)



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
	_float m_fMaxSpeed = 1.3f;
	_float m_fCurrentSpeed = 1.1f;
	_float m_fAccel = 0.f;

private:
	void Call_AnimationEnd();
protected:
	virtual void OnDestroy() override;
	void Free();

};

END




