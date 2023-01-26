#include "stdafx.h"
#include "Skill_Hammer.h"
#include "ClientComponent_Utils.h"
#include "PlayerStateBase.h"
#include "CorvusStates/CorvusState_PS_Hammer.h"
#include "Status_Player.h"
#include "Requirement_Time.h"
#include "RequirementChecker.h"
#include "Requirement_PlayerStatusMana.h"
#include "GameManager.h"
#include "CorvusStates/CorvusState_PS_Hammer_Upgrade.h"


GAMECLASS_C(CSkill_Hammer)
CLONE_C(CSkill_Hammer, CComponent)


void CSkill_Hammer::Start()
{
	__super::Start();
}

void CSkill_Hammer::Init_SkillInfo()
{
	USE_START(CSkill_Hammer);


	m_bUseAble = true;

	m_eSkillName = SKILL_NAME::SKILL_HAMMER;
	m_eSkillType = SKILL_TYPE::SKILL_ATK;

	m_fRequiredCost = 30.f;
	m_fSkillCoolDown = 15.f;

	m_pRequirementMana->Init_Req(m_fRequiredCost);
	m_pRequirementTime->Init_Req(0.f);
}

void CSkill_Hammer::Init_State()
{
	m_pSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_Hammer>();
	m_pExpansionSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_Hammer_Upgrade>();
}

void CSkill_Hammer::Free()
{

}
