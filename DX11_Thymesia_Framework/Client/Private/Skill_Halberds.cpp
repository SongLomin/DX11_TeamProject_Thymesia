#include "stdafx.h"
#include "Skill_Halberds.h"
#include "ClientComponent_Utils.h"
#include "PlayerStateBase.h"
#include "CorvusStates/CorvusState_PS_Halberds.h"
#include "CorvusStates/CorvusState_PS_Halberds_Upgrade.h"

#include "Status_Player.h"
#include "Requirement_Time.h"
#include "RequirementChecker.h"
#include "Requirement_PlayerStatusMana.h"
#include "GameManager.h"
#include "PlayerSkill_System.h"


GAMECLASS_C(CSkill_Halberds)
CLONE_C(CSkill_Halberds, CComponent)


void CSkill_Halberds::Start()
{
	__super::Start();
}

void CSkill_Halberds::Init_SkillInfo()
{
	USE_START(CSkill_Halberds);

	m_bUseAble = true;

	m_eSkillName = SKILL_NAME::SKILL_HALBERDS;
	m_eSkillType = SKILL_TYPE::SKILL_ATK;

	m_fRequiredCost = 30.f;
	m_fSkillCoolDown = 10.f;

	m_pRequirementMana->Init_Req(m_fRequiredCost);
	m_pRequirementTime->Init_Req(0.f);
}

void CSkill_Halberds::Init_State()
{
	m_pSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_Halberds>();
	m_pExpansionSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_Halberds_Upgrade>();
}

void CSkill_Halberds::Free()
{
}
