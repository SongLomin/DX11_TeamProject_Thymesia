#include "stdafx.h"
#include "Skill_Scythe.h"
#include "Skill_Hammer.h"
#include "ClientComponent_Utils.h"
#include "PlayerStateBase.h"
#include "CorvusStates/CorvusState_PS_Scythe.h"
#include "CorvusStates/CorvusState_PS_Scythe_Upgrade.h"
#include "Status_Player.h"
#include "Requirement_Time.h"
#include "RequirementChecker.h"
#include "Requirement_PlayerStatusMana.h"
#include "GameManager.h"

GAMECLASS_C(CSkill_Scythe)
CLONE_C(CSkill_Scythe, CComponent)


void CSkill_Scythe::Start()
{
	__super::Start();
}

void CSkill_Scythe::Init_SkillInfo()
{
	USE_START(CSkill_Scythe)

	m_bUseAble = true;

	m_eSkillName = SKILL_NAME::SKILL_SCYTHE;
	m_eSkillType = SKILL_TYPE::SKILL_ATK;

	m_fRequiredCost = 30.f;
	m_fSkillCoolDown = 15.f;

	m_pRequirementMana->Init_Req(m_fRequiredCost);
	m_pRequirementTime->Init_Req(0.f);
}

void CSkill_Scythe::Init_State()
{
	m_pSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_Scythe>();
	m_pExpansionSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_Scythe_Upgrade>();

}

void CSkill_Scythe::Free()
{

}
