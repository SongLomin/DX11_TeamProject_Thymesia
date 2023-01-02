#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUrdBossState_IdleStates abstract:
	public CUrdBossStateBase
{
	GAMECLASS_H(CUrdBossState_IdleStates)


public:
	 void      Set_StepFarCount(_int iStepFarCount) { m_iStepFarCount = iStepFarCount; }
	 void      Set_StepCloseCount(_int iStepCloseCount) { m_iStepCloseCount = iStepCloseCount; }
	 void      Set_Attack(_bool bAttack) { m_bAttack = bAttack; }
	 void      Set_WalkStart(_bool bWalkStart) { m_bWalkStart = bWalkStart; }
	 void      Set_SkillCount(_int iSkillCount) { m_iSkillCount = iSkillCount; }
	 void      Set_NoParryAttack(_bool bNoParryAttack) { m_bNoParryAttack = bNoParryAttack; }
	 void      Set_SpecailAttack(_bool bSpecailAttack) { m_bSpecailAttack = bSpecailAttack; }
	 void      Set_PhaseTwoStart(_bool bPhaseTwoStart) { m_bPhaseTwoStart = bPhaseTwoStart; }
	 void      Set_SkillStart(_bool bSkillStart) { m_bSkillStart = bSkillStart; }
	 void      Set_StepClose(_bool bStepClose) { m_bStepClose = bStepClose; }
	 void      Set_TurnCheck(_bool TurnCheck) { m_bTurnCheck = TurnCheck; }

	const _int Get_SkillCount() { return m_iSkillCount; }
protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

protected:
	
	_int      m_iStepFarCount = 0;
	_int      m_iStepCloseCount = 0;
	_int      m_iSkillCount = 0;

	_bool     m_bStepClose = false;
	_bool     m_bAttack = false;
	_bool     m_bNoParryAttack = false;
	_bool     m_bSpecailAttack = false;
	_bool     m_bWalkStart = false;
	_bool     m_bPhaseTwoStart = false;
	_bool     m_bSkillStart = false;
	_bool     m_bTurnCheck = false;


protected:
	void Free();

};

END






