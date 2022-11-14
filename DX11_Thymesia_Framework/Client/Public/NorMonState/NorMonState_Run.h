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
	_bool  m_bRunCheck = false; // ���̵鿡�� ������ ������ 8������ �ѹ������� �����̰� �ٽþ��̵�ΰ��� �ٽ÷����� �״������� �����ϰԲ�«
	// 8���� ���������� �޽� ������������ Ʈ��ιٲ��� 
protected:
	void Free();

};

END
