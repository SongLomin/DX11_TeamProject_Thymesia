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

	m_pRequirementChecker = CRequirementChecker::Create();
	m_pRequirementTime = CRequirementBase::Create< CRequirement_Time>();

	return S_OK;
}

void CStolenSkill::UseSkill()
{
	if (m_bUseAble == false || !m_pSkillState.lock())
	{
		return;
	}

	m_bUseAble = false;

	CClientComponent_Utils::ConvertOwnerToPlayer(m_pOwner).lock()->Change_State(m_pSkillState);

	m_pRequirementTime->Init_Req(m_fSkillCoolDown);
	m_pRequirementChecker->Add_Requirement(m_pRequirementTime);

	Start_Skill();

	m_eSkillName = SKILL_NAME::SKILL_END;
	m_pSkillState = weak_ptr<CPlayerStateBase>();

}

void CStolenSkill::OnStealSkill(weak_ptr<CSkill_Base> pSkill)
{
	m_eSkillName = pSkill.lock()->Get_SkillName();
	m_fSkillCoolDown = pSkill.lock()->Get_SkillCoolDown();
	m_pSkillState = pSkill.lock()->Get_SkillState();
}


void CStolenSkill::Free()
{

}
