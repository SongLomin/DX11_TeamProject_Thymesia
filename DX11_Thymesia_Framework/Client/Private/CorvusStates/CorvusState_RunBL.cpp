#include "stdafx.h"
#include "CorvusStates/CorvusState_RunBL.h"
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "PhysXController.h"


GAMECLASS_C(CCorvusState_RunBL);
CLONE_C(CCorvusState_RunBL, CComponent)



HRESULT CCorvusState_RunBL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_RunBL::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	
	return S_OK;
}



void CCorvusState_RunBL::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_RunBL");
}

void CCorvusState_RunBL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	PxControllerFilters Filters;

	m_pPhysXControllerCom.lock()->MoveWithRotation({ -m_fCurrentSpeed * fTimeDelta, 0.f, -m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);

	//m_pTransformCom.lock()->Go_Straight(m_fCurrentSpeed * fTimeDelta, m_pNaviCom);
}

void CCorvusState_RunBL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Rotation_TargetToLookDir();

	Check_AndChangeNextState();
}

void CCorvusState_RunBL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_fCurrentSpeed = m_fMaxSpeed / 2.f;

	PxControllerCollisionFlags Flags = Get_OwnerCharacter().lock()->Get_LastCollisionFlags();

	//if (!(Flags & PxControllerCollisionFlag::eCOLLISION_DOWN))
	//{
	//	// 떨어지고 있는 중
	//}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: Run -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_RunBL::OnStateEnd()
{
	__super::OnStateEnd();

}

void CCorvusState_RunBL::Free()
{
}

_bool CCorvusState_RunBL::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;



	if (KEY_INPUT(KEY::S, KEY_STATE::HOLD)
		&& KEY_INPUT(KEY::A, KEY_STATE::HOLD))
	{
		if (Check_RequirementAVoidState())
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_AVoidL>();
			return true;
		}

	}

	
		if (Check_RequirementAttackState())
		{

			Get_OwnerPlayer()->Change_State<CCorvusState_LAttack1>();
			return true;
		}

		if (Check_RequirementParryState())
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_Parry1>();
			return true;
		}

		if (Check_RequirementClawAttackState())
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackTab>();
			return true;
		}
	


		//if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
		//{
		//	Get_OwnerPlayer()->Change_State<CCorvusState_AVoid>();
		//	return true;
		//}
	

	


	

	if (!KEY_INPUT(KEY::S, KEY_STATE::HOLD)
		|| !KEY_INPUT(KEY::A, KEY_STATE::HOLD))
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
		return true;
	}

	

	if(Check_RequirementHealingState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_BasicHealing>();
		return true;
	}




	

	return false;
}

