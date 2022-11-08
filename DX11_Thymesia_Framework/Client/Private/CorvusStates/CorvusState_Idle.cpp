#include "stdafx.h"
#include "CorvusStates/CorvusState_Idle.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_Idle);
CLONE_C(CCorvusState_Idle, CComponent)

HRESULT CCorvusState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CCorvusState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iAnimIndex = 3;
	return S_OK;
}

void CCorvusState_Idle::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	
}

void CCorvusState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("Bip001");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir);
}

void CCorvusState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "LuxiyaState: Stand -> OnStateStart" << endl;

#endif
}

void CCorvusState_Idle::OnStateEnd()
{
	__super::OnStateEnd();
}

_bool CCorvusState_Idle::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//if (Check_RequirementUltimateState())
	//{
	//	Rotation_NearToLookDir();
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_UltimateSkill>();
	//	return true;
	//}
	//
	//if (Check_RequirementAttackState())
	//{
	//	if (!Rotation_InputToLookDir())
	//		Rotation_NearToLookDir();
	//
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_Attack>();
	//	Get_OwnerPlayer()->Get_Component<CLuxiyaState_Attack>().lock()->Play_AttackWithIndex(0);
	//	return true;
	//}
	//
	//if (Check_RequirementDashState())
	//{
	//	Rotation_InputToLookDir();
	//	//Open_HyperSpace();
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_Dash>();
	//	return true;
	//}
	if (Check_RequirementRunState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
		return true;
	}

	else if (Check_RequirementJoggingState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_JoggingStart>();
		return true;
	}


	else if (Check_RequirementSprintState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_SprintStart>();
		return true;
	}
		
		return false;
}


	void CCorvusState_Idle::Free()
	{
	}


