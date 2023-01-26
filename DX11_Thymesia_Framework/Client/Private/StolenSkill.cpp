#include "stdafx.h"
#include "StolenSkill.h"
#include "RequirementChecker.h"
#include "Requirement_Time.h"
#include "ClientComponent_Utils.h"
#include "GameManager.h"
#include "Player.h"
#include "Character.h"
#include "PlayerStateBase.h"

GAMECLASS_C(CStolenSkill)
CLONE_C(CStolenSkill, CComponent)


HRESULT CStolenSkill::Initialize(void* pArg)
{
	USE_START(CSkill_Base);

	m_eSkillName = SKILL_NAME::SKILL_END;

	m_pRequirementChecker = CRequirementChecker::Create();
	m_pRequirementTime = CRequirementBase::Create< CRequirement_Time>();


	m_pExpansionChecker = CRequirementChecker::Create();
	m_pExpansionTime = CRequirementBase::Create<CRequirement_Time>();

	m_pExpansionTime->Init_Req(m_fExpansionTime);

	return S_OK;
}

void CStolenSkill::UseSkill()
{
	if (!m_pSkillState.lock())
	{
		return;
	}
	__super::UseSkill();
}

void CStolenSkill::OnStealSkill(weak_ptr<CSkill_Base> pSkill)
{
	m_eSkillName = pSkill.lock()->Get_SkillName();
	m_fSkillCoolDown = pSkill.lock()->Get_SkillCoolDown();
	m_pSkillState = pSkill.lock()->Get_SkillState();
	m_pExpansionSkillState = pSkill.lock()->Get_ExpansionSkillState();

}

void CStolenSkill::Start_Skill(_bool bExpansion)
{
	__super::Start_Skill(bExpansion);

	m_eSkillName = SKILL_NAME::SKILL_END;
	m_pSkillState = weak_ptr<CPlayerStateBase>();
	m_pExpansionSkillState = weak_ptr<CPlayerStateBase>();
}


void CStolenSkill::Free()
{

}
