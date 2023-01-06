#pragma once
#include "ClientComponent.h"

BEGIN(Client)

class CSkill_Base;
class CRequirementChecker;
class CRequirement_Time;
class CStolenSkill;


class CPlayerSkill_System : public CClientComponent
{
public:
	typedef enum class socketType
	{
		SOCKET_MAIN = 0, SOCKET_SUB, SOCKET_END
	}SOCKET_TYPE;

public:
	GAMECLASS_H(CPlayerSkill_System);
	CLONE_H(CPlayerSkill_System, CComponent)


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual void Start() override;

public:
	void					ResetAllSkillCoolDown();

public:
	void					UseMainSKill();
	void					UseStealSKill();

	void					SwapSkillMaintoSub();
	HRESULT					OnStealMonsterSkill(MONSTERTYPE In_eMonsterType);

public:
	void			UnBindSkill(SOCKET_TYPE eType);
	void			OnChangeSkill(weak_ptr<CSkill_Base> pSkill, SOCKET_TYPE eType);
	void			OnChangeSkill(SKILL_NAME eSkillName, SOCKET_TYPE eType);


	void			RegisterSkill(SKILL_NAME eName, weak_ptr<CSkill_Base> pSkill);


public:
	FDelegate<weak_ptr<CSkill_Base>>	Callback_OnChangeSkill[(_uint)SOCKET_TYPE::SOCKET_END];


	FDelegate<weak_ptr<CSkill_Base>>	Callback_OnStealSkill;





private:
	void			SetUp_SkillMapFromMonsterType();

	void			Tick_SkillList(_float fTimeDelta);

private:
	shared_ptr<CRequirementChecker> m_pRequirementChecker;
	shared_ptr<CRequirement_Time>	m_pRequirement_Time;


	weak_ptr<CSkill_Base>		m_SkillList[(_uint)SOCKET_TYPE::SOCKET_END];
	weak_ptr<CStolenSkill>		m_pStealSkill;

	typedef map<MONSTERTYPE, weak_ptr<CSkill_Base>> MONSTERSKILLMAP;
	map<MONSTERTYPE, weak_ptr<CSkill_Base>>	m_MonsterSkillMap;


	typedef map<SKILL_NAME, weak_ptr<CSkill_Base>> SKILLNAMEMAP;
	map<SKILL_NAME, weak_ptr<CSkill_Base>>	m_SkillNameMap;

public:
	
};


END