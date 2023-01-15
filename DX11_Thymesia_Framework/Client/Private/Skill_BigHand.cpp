#include "stdafx.h"
#include "Skill_BigHand.h"
#include "ClientComponent_Utils.h"
#include "PlayerStateBase.h"
#include "CorvusStates/CorvusState_PS_BigHand.h"	
#include "Status_Player.h"
#include "Requirement_Time.h"
#include "RequirementChecker.h"
#include "Requirement_PlayerStatusMana.h"
#include "GameManager.h"
#include "PlayerSkill_System.h"



GAMECLASS_C(CSkill_BigHand)
CLONE_C(CSkill_BigHand, CComponent)


void CSkill_BigHand::Start()
{
	__super::Start();
}

void CSkill_BigHand::Init_SkillInfo()
{
	USE_START(CSkill_BigHand);

	m_bUseAble = true;

	m_eSkillName = SKILL_NAME::SKILL_BIGHAND;
	m_eSkillType = SKILL_TYPE::SKILL_ATK;

	m_fRequiredCost = 30.f;
	m_fSkillCoolDown = 10.f;

	m_pRequirementMana->Init_Req(m_fRequiredCost);
	m_pRequirementTime->Init_Req(0.f);
}

void CSkill_BigHand::Init_State()
{
	m_pSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_BigHand>();
}

void CSkill_BigHand::Free()
{
}
