#pragma once
#include "BossBat/BatBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBatBossState_HellIdle :
	public CBatBossStateBase
{
	GAMECLASS_H(CBatBossState_HellIdle);
	CLONE_H(CBatBossState_HellIdle, CComponent)
		SHALLOW_COPY(CBatBossState_HellIdle)


public:
	void    Set_ChestCheck(_bool ChestCheck) { m_bChestCheck =  ChestCheck; }
	void    Set_TurnCheck(_bool TurnCheck) { m_bTurnCheck = TurnCheck; }
	void    Set_ChargeCount(_uint ChargeCount) { m_iChargeCount += ChargeCount; }
	void    Set_AttackCount(_uint AttackCount) { m_iAttackCount += AttackCount; }
	void    Set_HeelScream(_bool bHeelScream) { m_bHeelScream = bHeelScream; }
	void    Set_ZeroAttackCount(_uint ZeroAttackCount) { m_iAttackCount = ZeroAttackCount; }
	void    Set_ZeroChargeCount(_uint ZeroChargeCount) { m_iChargeCount = ZeroChargeCount; }

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
	//필요한게 charge가 몇번쨰인지 
	//charge상태인지 charge 상태가아닌지
	//내가어택읆쳐번했는지 
	_uint    m_iChargeCount = 0;
	_uint    m_iAttackCount = 0;
	_bool    m_bTurnCheck = false;
	_bool    m_bHeelScream = false;
	_bool    m_bChestCheck = false;


private:
	void Call_AnimationEnd();
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






