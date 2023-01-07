#include "stdafx.h"
#include "Skill_Axe.h"
#include "ClientComponent_Utils.h"
#include "PlayerStateBase.h"
#include "CorvusStates/CorvusState_PS_Axe.h"
#include "Status_Player.h"
#include "Requirement_Time.h"
#include "RequirementChecker.h"
#include "Requirement_PlayerStatusMana.h"
#include "GameManager.h"
#include "PlayerSkill_System.h"



GAMECLASS_C(CSkill_Axe)
CLONE_C(CSkill_Axe, CComponent)


void CSkill_Axe::Start()
{
	__super::Start();
}

void CSkill_Axe::Init_SkillInfo()
{
	USE_START(CSkill_Axe);


	m_bUseAble = true;

	m_eSkillName = SKILL_NAME::SKILL_AXE;
	m_eSkillType = SKILL_TYPE::SKILL_ATK;

	m_fRequiredCost = 30.f;
	m_fSkillCoolDown = 10.f;

	m_pRequirementMana->Init_Req(m_fRequiredCost);
	m_pRequirementTime->Init_Req(0.f);
}

void CSkill_Axe::Init_State()
{
	m_pSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_Axe>();
}

void CSkill_Axe::Free()
{
}
