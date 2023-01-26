#include "stdafx.h"
#include "CorvusStates/CorvusState_SprintStart.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_SprintStart);
CLONE_C(CCorvusState_SprintStart, CComponent)

HRESULT CCorvusState_SprintStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_SprintStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	
	return S_OK;
}

void CCorvusState_SprintStart::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD1_Sprint_Start");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_SprintStart::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_SprintStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_SprintStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_SprintStart::OnDisable()
{

}

void CCorvusState_SprintStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif
	
	
}

void CCorvusState_SprintStart::OnStateEnd()
{
	__super::OnStateEnd();
	
}

void CCorvusState_SprintStart::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Sprint>();

}

void CCorvusState_SprintStart::Free()
{

}

void CCorvusState_SprintStart::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_SprintStart::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_SprintStart::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//if (Check_RequirementUltimateState())
	//{
	//	Rotation_NearToLookDir();
	//	Get_OwnerPlayer()->Change_State<CNorMonState_UltimateSkill>();
	//	return true;
	//}
	//
	//if (Check_RequirementAttackState())
	//{
	//	if (!Rotation_InputToLookDir())
	//		Rotation_NearToLookDir();
	//
	//	Get_OwnerPlayer()->Change_State<CNorMonState_Attack>();
	//	Get_OwnerPlayer()->Get_Component<CNorMonState_Attack>().lock()->Play_AttackWithIndex(0);
	//	return true;
	//}
	
	if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
	{
		if (!KEY_INPUT(KEY::W, KEY_STATE::HOLD)
			&& !KEY_INPUT(KEY::A, KEY_STATE::HOLD)
			&& !KEY_INPUT(KEY::S, KEY_STATE::HOLD)
			&& !KEY_INPUT(KEY::D, KEY_STATE::HOLD))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
			return true;
		}
	}


	if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_AVoid>();
		return true;
	}

	if (Check_RequirementAttackState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_SprintAttack>();
		return true;
	}

	if (Check_RequirementParryState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_Parry1>();
		return true;
	}

	if (Check_RequirementClawAttackState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackTab>();
		return true;
	}

	return false;
}


