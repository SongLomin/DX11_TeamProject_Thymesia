#pragma once
#include "NorMonsterStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_Run :
	public CNorMonsterStateBase
{
	GAMECLASS_H(CNorMonState_Run);
	CLONE_H(CNorMonState_Run, CComponent)
		SHALLOW_COPY(CNorMonState_Run)

public:
	void  Set_ClosePlayer(_bool ClosePlayer) { m_bClosePlayer = ClosePlayer; }
	void  Set_MonIdleType(NORMONSTERIDLETYPE IDLETYPE);

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
	_float m_fCurrentSpeed = 4.2f;
	_float m_fAccel = 0.f;
	_bool  m_bRunCheck = false; // ���̵鿡�� ������ ������ 8������ �ѹ������� �����̰� �ٽþ��̵�ΰ��� �ٽ÷����� �״������� �����ϰԲ�«
	// 8���� ���������� �޽� ������������ Ʈ��ιٲ��� 

	//TODO �߸�
	_bool m_bClosePlayer = true;
private:
	void Call_AnimationEnd();
protected:
	virtual void OnDestroy() override;
	void Free();

};

END
