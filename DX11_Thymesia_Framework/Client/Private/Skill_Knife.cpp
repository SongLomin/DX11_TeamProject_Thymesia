#include "stdafx.h"
#include "Skill_Knife.h"
#include "ClientComponent_Utils.h"
#include "PlayerStateBase.h"
#include "CorvusStates/CorvusState_PS_Knife.h"
#include "Status_Player.h"
#include "Requirement_Time.h"
#include "RequirementChecker.h"
#include "Requirement_PlayerStatusMana.h"
#include "GameManager.h"

GAMECLASS_C(CSkill_Knife)
CLONE_C(CSkill_Knife, CComponent)

void CSkill_Knife::Start()
{
	__super::Start();
}


void CSkill_Knife::Init_SkillInfo()
{
	USE_START(CSkill_Knife);


	m_bUseAble = true;

	m_eSkillName = SKILL_NAME::SKILL_KNIFE;
	m_eSkillType = SKILL_TYPE::SKILL_ATK;

	m_fRequiredCost = 30.f;
	m_fSkillCoolDown = 5.f;

	m_pRequirementMana->Init_Req(m_fRequiredCost);
	m_pRequirementTime->Init_Req(0.f);
}

void CSkill_Knife::Init_State()
{
	m_pSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_Knife>();

}

void CSkill_Knife::Free()
{
}
