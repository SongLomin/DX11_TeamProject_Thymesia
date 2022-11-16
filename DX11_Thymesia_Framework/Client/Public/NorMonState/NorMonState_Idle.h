#pragma once
#include "AIStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_Idle :
	public CAIStateBase
{
	GAMECLASS_H(CNorMonState_Idle);
	CLONE_H(CNorMonState_Idle, CComponent)
		SHALLOW_COPY(CNorMonState_Idle)

public:
	void Set_MonIdleType(NORMONSTERIDLETYPE IDLETYPE);
	void Set_OneCheck(_bool OneCheck) { m_bOneCheck = OneCheck; }


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


private: // ���̵鿡�������ºҰ�
	_bool     m_bFirstRun = false; // 6���ٸֶ� �������Ѿƿ��ϱ� 6���� ��������� ���Լ� �ۿ� ���� �����Ÿ��־����� ���Լ� false�θ����
	_bool     m_bReturnStartPosition = false;


	//TODO �߸�
	_bool    m_bPlayerColision = false;
	_bool    m_bOneCheck = false;


protected:
	void Free();

};

END