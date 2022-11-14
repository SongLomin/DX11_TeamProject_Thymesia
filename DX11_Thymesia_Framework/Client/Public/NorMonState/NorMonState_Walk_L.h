#pragma once
#include "AIStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_Walk_L :
	public CAIStateBase
{
	GAMECLASS_H(CNorMonState_Walk_L);
	CLONE_H(CNorMonState_Walk_L, CComponent)
		SHALLOW_COPY(CNorMonState_Walk_L)



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
	_float m_fMaxSpeed = 1.f;
	_float m_fCurrentSpeed = 0.5f;
	_float m_fAccel = 0.f;

private:
	void Call_AnimationEnd();
protected:
	virtual void OnDestroy() override;
	void Free();

};

END


