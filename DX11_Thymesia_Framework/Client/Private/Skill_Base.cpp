#include "stdafx.h"
#include "Skill_Base.h"
#include "PlayerStateBase.h"
#include "Status_Player.h"
#include "Player.h"
#include "ClientComponent_Utils.h"
#include "CCorvus.h"
#include "RequirementChecker.h"
#include "Requirement_PlayerStatusMana.h"
#include "Requirement_Time.h"
#include "GameManager.h"
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

	USE_START(CSkill_Base);

	m_pRequirementChecker = CRequirementChecker::Create();

	m_pRequirementMana = CRequirementBase::Create< CRequirement_PlayerStatusMana>();
	m_pRequirementTime = CRequirementBase::Create< CRequirement_Time>();

	m_pRequirementChecker->Add_Requirement(m_pRequirementMana);
	m_pRequirementChecker->Add_Requirement(m_pRequirementTime);

	Init_SkillInfo();
	Init_State();

	return S_OK;
}

void CSkill_Base::Start()
{
	__super::Start();

}

void CSkill_Base::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bUseAble)
	{
		return;
	}	

	_bool isUseAble = Is_UseAble();

	if (isUseAble)//��Ÿ���� �� ���Ҵٸ�
	{
		if (m_bUseAble == false)//���������� ��� �Ұ����̾��ٸ�
		{
			Callback_EndCoolDown();
		}
	}
	m_bUseAble = isUseAble;
}

void CSkill_Base::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

_bool CSkill_Base::Is_UseAble()
{
	weak_ptr<CStatus_Player> pPlayerStatus = m_pOwner.lock()->Get_Component<CStatus_Player>();

	if (!pPlayerStatus.lock())
	{
		DEBUG_ASSERT;
		return false;
	}
	if (!m_pRequirementChecker->Check_Requirments())//���ǿ� �������� �ʴ´�. ->��ų���� �����ְų�, ������ ������.
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
	Start_Skill();
}

_float CSkill_Base::Get_RatioCoolDown()
{
	return m_pRequirementTime->Get_RatioTime();
}

void CSkill_Base::Reset_Skill()
{
	Init_SkillInfo();
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

void CSkill_Base::Start_Skill()
{
	Callback_StartSkill();
	CClientComponent_Utils::ConvertOwnerToPlayer(m_pOwner).lock()->Change_State(m_pSkillState);
	m_bUseAble = false;
	GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status().lock()->Consumed_Mana(m_fRequiredCost);

	m_pRequirementTime->Init_Req(m_fSkillCoolDown);
}

void CSkill_Base::End_Skill()
{
	m_bUseAble = true;
	Callback_EndCoolDown();
}

void CSkill_Base::Free()
{
	
}
