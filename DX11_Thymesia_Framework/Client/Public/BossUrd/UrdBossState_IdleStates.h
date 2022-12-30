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
	 void      Set_StepCount(_int iStepCount) { m_iStepCount = iStepCount; }
	 void      Set_AttackCount(_bool bAttack) { m_bAttack = bAttack; }
	 void      Set_WalkCount(_int iWalkCount) { m_iWalkCount = iWalkCount; }
	 void      Set_SkillCount(_int iSkillCount) { m_iSkillCount = iSkillCount; }
	 void      Set_NoParryAttack(_int iNoParryAttack) { m_iNoParryAttack = iNoParryAttack; }
	 void      Set_ThreeAttack(_bool bThreeAttack) { m_bThreeAttack = bThreeAttack; }
	 void      Set_SkillStart(_bool bSkillStart) { m_bSkillStart = bSkillStart; }
protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

protected:
	_int      m_iStepCount = 0;
	_bool     m_bAttack = 0;
	_int      m_iNoParryAttack = 0;
	_bool     m_bThreeAttack = false;
	_int      m_iWalkCount = 0;
	_int      m_iSkillCount = 0;
	_bool     m_bSkillStart = false;




protected:
	void Free();

};

END






