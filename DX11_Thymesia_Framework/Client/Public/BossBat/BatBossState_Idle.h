#pragma once
#include "BossBat/BatBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBatBossState_Idle :
	public CBatBossStateBase
{
	GAMECLASS_H(CBatBossState_Idle);
	CLONE_H(CBatBossState_Idle, CComponent)
		SHALLOW_COPY(CBatBossState_Idle)


public:
	void    Set_TurnCheck(_bool TurnCheck) { m_bTurnCheck = TurnCheck; }
	void    Set_ChargeCount(_uint ChargeCount) { m_iChargeCount += ChargeCount; }
	void    Set_AttackCount(_uint AttackCount) { m_iAttackCount += AttackCount; }
	void    Set_HeelScream(_bool bHeelScream) { m_bHeelScream = bHeelScream; }

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
	//�ʿ��Ѱ� charge�� ��������� 
	//charge�������� charge ���°��ƴ���
	//�������ß��Ĺ��ߴ��� 
	_uint    m_iChargeCount = 0;
	_uint    m_iAttackCount = 0;
	_bool    m_bCheckCharge = false;
	_bool    m_bTurnCheck = false;
	_bool    m_bHeelScream = false;
	

private:
//	void Call_AnimationEnd();
protected:
	//virtual void OnDestroy() override;
	void Free();

};

END






