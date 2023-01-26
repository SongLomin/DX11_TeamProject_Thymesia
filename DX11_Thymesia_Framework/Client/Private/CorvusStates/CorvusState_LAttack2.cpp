#include "stdafx.h"
#include "CorvusStates/CorvusState_LAttack2.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CCorvusState_LAttack2);
CLONE_C(CCorvusState_LAttack2, CComponent)

HRESULT CCorvusState_LAttack2::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CCorvusState_LAttack2::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_vShakingOffSet = _float3(0.6f, -1.f, 0.f);

	return S_OK;
}

void CCorvusState_LAttack2::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_LAttack2");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_LAttack2::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_LAttack2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	Attack();
}

void CCorvusState_LAttack2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_InputNextAttack();

	Check_AndChangeNextState();
}

void CCorvusState_LAttack2::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;
	cout << "CorvusState: Attack2 -> AnimEnd" << endl;
	Get_OwnerPlayer()->Change_State<CCorvusState_LAttack3>();

	
}

void CCorvusState_LAttack2::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{


	


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_LAttack2::Attack()
{

}

void CCorvusState_LAttack2::Check_InputNextAttack()
{
	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return;
	}

	m_IsNextAttack = true;

	//case 3:
	//	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
	//	{
	//		m_IsNextAttack = true;
	//	}
	//	break;
	//
	//case 5:
	//	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
	//	{
	//		m_IsNextAttack = true;
	//	}
	//	break;
	

}



void CCorvusState_LAttack2::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	if (!m_pModelCom.lock().get())
	{
		
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pPhysXControllerCom.lock()->Callback_ControllerHit +=
		bind(&CCorvusState_LAttack2::Call_OtherControllerHit, this, placeholders::_1);

	//m_iAttackIndex = 7;
	//m_iEndAttackEffectIndex = -1;
	

	//Disable_Weapons();


	

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "CorvusState: Attack2 -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_LAttack2::OnStateEnd()
{
	__super::OnStateEnd();

	//Disable_Weapons();
	m_IsNextAttack = false;
	m_pPhysXControllerCom.lock()->Callback_ControllerHit +=
		bind(&CCorvusState_LAttack2::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_LAttack2::OnEventMessage(_uint iArg)
{
	//__super::OnEventMessage(iArg);
	//
	//if ((_uint)EVENT_TYPE::ON_FIRSTHIT == iArg)
	//{
	//	switch (m_pModelCom.lock()->Get_CurrentAnimationIndex())
	//	{
	//	case 0:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack01.wav"), 1.f);
	//		break;
	//
	//	case 1:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack02.wav"), 1.f);
	//		break;
	//
	//	case 2:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack03.wav"), 1.f);
	//		break;
	//
	//	case 3:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack04.wav"), 1.f);
	//		break;
	//
	//	case 4:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack05.wav"), 1.f);
	//		break;
	//
	//	case 5:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack06.wav"), 1.f);
	//		break;
	//	}
	//}

}

void CCorvusState_LAttack2::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_LAttack2::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_LAttack2::Free()
{
	
}

_bool CCorvusState_LAttack2::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 27)
	{
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
	}



	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 53)
	{
		if (Check_RequirementAVoidState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_AVoid>();
			return true;
		}

		if (Check_RequirementRunState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
			return true;
		}
	}

	if (Check_RequirementNextAttackState())
	{
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
				if (!Rotation_InputToLookDir())
					Rotation_TargetToLookDir();

				Get_OwnerPlayer()->Change_State<CCorvusState_LAttack3>();

			}
			return true;

		}
	}

	if (Check_RuquireMnetFirstAttackState())
	{
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
				if (!Rotation_InputToLookDir())
					Rotation_TargetToLookDir();

				Get_OwnerPlayer()->Change_State<CCorvusState_LAttack1>();

			}
		}
		
	}

	



	return false;
}

_bool CCorvusState_LAttack2::Check_RequirementNextAttackState()
{
	_uint iTargetKeyFrameFirst = 12;
	_uint iTargetKeyFrameSecond = 50;
	

	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameFirst, iTargetKeyFrameSecond) && m_IsNextAttack)
	{
		return true;
	}

	return false;
}

_bool CCorvusState_LAttack2::Check_RuquireMnetFirstAttackState()
{
	_uint iTargetKeyFrameMin = 51;
	_uint iTargetKeyFrameMax = 80;

	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax)&& m_IsNextAttack)
	{
		return true;
	}

	return false;
}

