#pragma once
#include "BossBat/BatBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBatBossState_IdleStates abstract:
	public CBatBossStateBase
{
	GAMECLASS_H(CBatBossState_IdleStates)


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
	_uint    m_iChargeCount = 0;
	_uint    m_iAttackCount = 0;
	_bool    m_bTurnCheck = false;
	_bool    m_bHeelScream = false;
	_bool    m_bChestCheck = false;
	


protected:

	void Free();

};

END






