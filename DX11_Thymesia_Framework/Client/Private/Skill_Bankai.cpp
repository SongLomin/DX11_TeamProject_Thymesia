#include "stdafx.h"
#include "Skill_Bankai.h"
#include "ClientComponent_Utils.h"
#include "PlayerStateBase.h"
#include "CorvusStates/CorvusState_PS_CaneSword_Start.h"
#include "Status_Player.h"
#include "Requirement_Time.h"
#include "RequirementChecker.h"
#include "Requirement_PlayerStatusMana.h"
#include "GameManager.h"
#include "PlayerSkill_System.h"


GAMECLASS_C(CSkill_Bankai)
CLONE_C(CSkill_Bankai, CComponent)


void CSkill_Bankai::Start()
{
	__super::Start();
}

void CSkill_Bankai::Init_SkillInfo()
{
	USE_START(CSkill_Bankai);


	m_bUseAble = true;

	m_eSkillName = SKILL_NAME::SKILL_BANKAI;
	m_eSkillType = SKILL_TYPE::SKILL_ATK;

	m_fRequiredCost = 100.f;
	m_fSkillCoolDown = 30.f;

	m_pRequirementMana->Init_Req(m_fRequiredCost);
	m_pRequirementTime->Init_Req(0.f);
}

void CSkill_Bankai::Init_State()
{
	m_pSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_CaneSword_Start>();
}

void CSkill_Bankai::Free()
{

}
