#include "stdafx.h"
#include "Skill_Base.h"
#include "PlayerStateBase.h"
#include "Status_Player.h"
#include "Player.h"
#include "ClientComponent_Utils.h"
#include "Corvus.h"
#include "RequirementChecker.h"
#include "Requirement_PlayerStatusMana.h"
#include "Requirement_Time.h"
#include "GameManager.h"
#include "UI_Utils.h"
#include "PlayerSkill_System.h"



GAMECLASS_C(CSkill_Base)
CLONE_C(CSkill_Base, CComponent)


HRESULT CSkill_Base::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CSkill_Base::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pRequirementChecker = CRequirementChecker::Create();
	
	m_fExpansionTime = 0.3f;


	m_pRequirementMana = CRequirementBase::Create< CRequirement_PlayerStatusMana>();
	m_pRequirementTime = CRequirementBase::Create< CRequirement_Time>();
	
	m_pRequirementChecker->Add_Requirement(m_pRequirementMana);


	m_pExpansionChecker = CRequirementChecker::Create();
	m_pExpansionTime = CRequirementBase::Create<CRequirement_Time>();

	m_pExpansionTime->Init_Req(m_fExpansionTime);


	Init_SkillInfo();
	Init_State();
	
	m_eRequirementSkillPiece = CUI_Utils::ConvertSkillNameToSkillPiece(m_eSkillName);

	RegisterThisSkillFromSkillSystem();


	return S_OK;
}

void CSkill_Base::Start()

{
	__super::Start();

}

void CSkill_Base::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_bool isUseAble = Is_UseAble();

	if (isUseAble)//쿨타임이 다 돌았다면
	{
		if (m_bUseAble == false)//전프레임이 사용 불가능이었다면
		{
			Callback_EndCoolDown();
		}
	}

	m_bUseAble = isUseAble;

	if (m_pExpansionChecker->Check_Requirments() && m_bInputedKey)
	{
		Start_Skill();
	}
}

void CSkill_Base::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

void CSkill_Base::RegisterThisSkillFromSkillSystem()
{
	if (m_eSkillName == SKILL_NAME::SKILL_END)
	{
		return;
	}

	m_pOwner.lock()->Get_Component<CPlayerSkill_System>().lock()->RegisterSkill(m_eSkillName, Weak_StaticCast<CSkill_Base>(m_this));
}

_bool CSkill_Base::Is_UseAble()
{
	weak_ptr<CStatus_Player> pPlayerStatus = m_pOwner.lock()->Get_Component<CStatus_Player>();

	if (!pPlayerStatus.lock())
	{
		DEBUG_ASSERT;
		return false;
	}
	if (!m_pRequirementChecker->Check_Requirments())//조건에 충족하지 않는다. ->스킬쿨이 돌고있거나, 마나가 부족함.
	{
		Callback_UpdateCoolDown(Get_RatioCoolDown());
		return false;
	}
	return true;
}

void CSkill_Base::UseSkill()
{
	if (m_bUseAble == false)
	{
		return;
	}
	if (!m_pExpansionSkillState.lock())
	{
		Start_Skill();
		return;
	}
	if (m_pExpansionChecker->size() > 0)
	{
		m_pExpansionChecker->Clear_Requirements();
		Start_Skill(true);
		return;
	}
	m_pExpansionTime->Init_Req(m_fExpansionTime);
	m_pExpansionChecker->Add_Requirement(m_pExpansionTime);
	m_bInputedKey = true;
}

_float CSkill_Base::Get_RatioCoolDown()
{
	return m_pRequirementTime->Get_RatioTime();
}

void CSkill_Base::Reset_Skill()
{
	Init_SkillInfo();
	Callback_EndCoolDown();
}

void CSkill_Base::Clear_Callback()
{
	//Safe_DelegateClear(Callback_StartSkill);
	//Safe_DelegateClear(Callback_UpdateCoolDown);
	//Safe_DelegateClear(Callback_EndCoolDown);

	Callback_StartSkill.Clear();
	Callback_UpdateCoolDown.Clear();
	Callback_EndCoolDown.Clear();
}

void CSkill_Base::Start_Skill(_bool bExapnsion)
{
	m_bUseAble = false;

	if (bExapnsion)
	{
		CClientComponent_Utils::ConvertOwnerToPlayer(m_pOwner).lock()->Change_State(m_pExpansionSkillState);
	}
	else
	{
		CClientComponent_Utils::ConvertOwnerToPlayer(m_pOwner).lock()->Change_State(m_pSkillState);

	}
	m_bInputedKey = false;
	GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status().lock()->Consumed_Mana(m_fRequiredCost);

	m_pRequirementTime->Init_Req(m_fSkillCoolDown);
	m_pRequirementChecker->Add_Requirement(m_pRequirementTime);

	Callback_StartSkill();
}

void CSkill_Base::End_Skill()
{
	m_bUseAble = true;
	Callback_EndCoolDown();
}

void CSkill_Base::Free()
{
	
}
