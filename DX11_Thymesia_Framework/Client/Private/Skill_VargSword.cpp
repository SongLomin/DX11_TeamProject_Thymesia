#include "stdafx.h"
#include "Skill_VargSword.h"
#include "ClientComponent_Utils.h"
#include "PlayerStateBase.h"
#include "CorvusStates/CorvusState_PS_VargSwordStart.h"
#include "Status_Player.h"
#include "Requirement_Time.h"
#include "RequirementChecker.h"
#include "Requirement_PlayerStatusMana.h"
#include "GameManager.h"

GAMECLASS_C(CSkill_VargSword)
CLONE_C(CSkill_VargSword, CComponent)


void CSkill_VargSword::Start()
{
	__super::Start();
}

void CSkill_VargSword::Init_SkillInfo()
{
	USE_START(CSkill_VargSword)

	m_bUseAble = true;

	m_eSkillName = SKILL_NAME::SKILL_VARGSWORD;
	m_eSkillType = SKILL_TYPE::SKILL_ATK;

	m_fRequiredCost = 30.f;
	m_fSkillCoolDown = 15.f;
	
	m_pRequirementMana->Init_Req(m_fRequiredCost);	
	m_pRequirementTime->Init_Req(0.f);
}

void CSkill_VargSword::Init_State()
{
	m_pSkillState = m_pOwner.lock()->Get_Component<CCorvusState_PS_VargSwordStart>();
}

void CSkill_VargSword::Free()
{
}
