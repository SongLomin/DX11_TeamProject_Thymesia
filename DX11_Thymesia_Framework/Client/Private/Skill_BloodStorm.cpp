#include "stdafx.h"
#include "Skill_BloodStorm.h"
#include "ClientComponent_Utils.h"
#include "PlayerStateBase.h"
#include "CorvusStates/CorvusState_PS_BatRoar.h"
#include "Status_Player.h"
#include "Requirement_Time.h"
#include "RequirementChecker.h"
#include "Requirement_PlayerStatusMana.h"
#include "GameManager.h"
#include "PlayerSkill_System.h"
#include "CorvusStates/CorvusState_PS_BatRoar_Upgrade.h"

GAMECLASS_C(CSkill_BloodStorm)
CLONE_C(CSkill_BloodStorm, CComponent)

void CSkill_BloodStorm::Start()
{
	__super::Start();
}

void CSkill_BloodStorm::Init_SkillInfo()
{
	USE_START(CSkill_BloodStorm);

	m_bUseAble = true;

	m_eSkillName = SKILL_NAME::SKILL_BLOODSTORM;
	m_eSkillType = SKILL_TYPE::SKILL_ATK;

	m_fRequiredCost = 30.f;
	m_fSkillCoolDown = 20.f;

	m_pRequirementMana->Init_Req(m_fRequiredCost);
	m_pRequirementTime->Init_Req(0.f);
}

void CSkill_BloodStorm::Init_State()
{
	m_pSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_BatRoar>();
	m_pExpansionSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_BatRoar_Upgrade>();
}

void CSkill_BloodStorm::Free()
{
}
