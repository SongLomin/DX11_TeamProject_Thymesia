#include "stdafx.h"
#include "CorvusStates/CorvusState_RunR.h"
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "PhysXController.h"


GAMECLASS_C(CCorvusState_RunR);
CLONE_C(CCorvusState_RunR, CComponent)



HRESULT CCorvusState_RunR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_RunR::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	
	return S_OK;
}



void CCorvusState_RunR::Start()
{
	__super::Start();

	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_RunR");
}

void CCorvusState_RunR::Tick(_float fTimeDelta)
{
	CPlayerStateBase::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);

	m_fTimeAcc += fTimeDelta;
	if (0.4f < m_fTimeAcc)
	{
		GET_SINGLE(CGameManager)->Add_WaterWave(m_pOwnerFromPlayer.lock()->Get_WorldPosition(), 0.1f, 9.f, 3.f);
		m_fTimeAcc = 0.f;
	}

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	PxControllerFilters Filters;

	m_pPhysXControllerCom.lock()->MoveWithRotation({ m_fCurrentSpeed * fTimeDelta, 0.f, 0.f }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);

	//m_pTransformCom.lock()->Go_Straight(m_fCurrentSpeed * fTimeDelta, m_pNaviCom);
}

void CCorvusState_RunR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Rotation_TargetToLookDir();

	Check_AndChangeNextState();
}

void CCorvusState_RunR::OnStateStart(const _float& In_fAnimationBlendTime)
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

void CCorvusState_RunR::OnStateEnd()
{
	__super::OnStateEnd();

}

void CCorvusState_RunR::Free()
{
}

_bool CCorvusState_RunR::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (KEY_INPUT(KEY::S, KEY_STATE::HOLD))
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_RunBR>();
		return true;
	}

	if (KEY_INPUT(KEY::W, KEY_STATE::HOLD))
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_RunFR>();
		return true;
	}




	
		if (Check_RequirementAttackState())
		{
			

			weak_ptr<CGameObject> pTargetObject;

			if (Check_RequirementExcuteState(pTargetObject))
			{
				_vector vTargetPos = pTargetObject.lock()->Get_Transform()->Get_Position();
				m_pTransformCom.lock()->LookAt2D(vTargetPos);
				Get_OwnerPlayer()->Change_State<CCorvusState_NorMob_Execution>();
				Get_OwnerPlayer()->Get_CurState().lock()->OnEventMessage(Weak_Cast<CBase>(pTargetObject));
			}
			else
			{

				Get_OwnerPlayer()->Change_State<CCorvusState_LAttack1>();

			}
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

	
	

	if (KEY_INPUT(KEY::D, KEY_STATE::HOLD)
		&& KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_AVoidR>();
		return true;
	}




	

	if (!KEY_INPUT(KEY::D, KEY_STATE::HOLD))
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
		return true;
	}


	if(Check_RequirementHealingState())
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_BasicHealing>();
		return true;
	}



	

	return false;
}

