#pragma once
#include "AIStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_Run :
	public CAIStateBase
{
	GAMECLASS_H(CNorMonState_Run);
	CLONE_H(CNorMonState_Run, CComponent)
		SHALLOW_COPY(CNorMonState_Run)



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
	_float m_fMaxSpeed = 8.f;
	_float m_fCurrentSpeed = 3.8f;
	_float m_fAccel = 0.f;
	_bool  m_bRunCheck = false; // 아이들에서 런으로 들어오고 8방향중 한방향으로 움직이고 다시아이들로가고 다시런가고 그다음에는 공격하게끔짬
	// 8방향 가기전에는 펄스 가고난다음에는 트루로바꿔줌 
protected:
	void Free();

};

END
