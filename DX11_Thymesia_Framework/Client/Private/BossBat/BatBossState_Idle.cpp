#include "stdafx.h"
#include "BossBat/BatBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_Idle);
CLONE_C(CBatBossState_Idle, CComponent)

HRESULT CBatBossState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Idle::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|IDLE");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Idle::Call_AnimationEnd, this);
}

void CBatBossState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CBatBossState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Atk_L01_2b>().lock()         ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Atk_R01_2b>().lock()         ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Bite_2>().lock()             ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Charge>().lock()             ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Car>().lock()                ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_JumpSmash_Chest>().lock()    ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_JumpSmash_ForwardL>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_JumpSmash_SmarhL>().lock()   ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_SonicBoom>().lock()          ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Hellscream>().lock()         ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Sp>().lock())
	{ 
		m_bTurnCheck = true;
	}



	m_pPhysXControllerCom.lock()->Enable_Gravity(true);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	
}	

void CBatBossState_Idle::OnStateEnd()
{
	__super::OnStateEnd();

}


void CBatBossState_Idle::Free()
{

}

void CBatBossState_Idle::Call_AnimationEnd()
{

}

void CBatBossState_Idle::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Idle::Call_AnimationEnd, this);
}

_bool CBatBossState_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	//6.5기준이로잡으면될듯
	_float fPToMDistance = Get_DistanceWithPlayer();



	int iRand = rand() % 2;

	switch (iRand)
	{
	case 0:
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.5f)
		{
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_Atk_L01_1>(0.05f);
			return true;
		}
	}
		break;
	case 1:
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.5f)
		{
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_Atk_R01_1>(0.05f);
			return true;
		}
	}
		break;
	}
   




	//_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리
	//
	//
	//if (m_bHeelScream)
	//{
	//	if (fPToMDistance >= 8.f)
	//	{
	//		if (m_bTurnCheck)
	//		{
	//			TurnMechanism();
	//		}
	//
	//		switch (m_iChargeCount)
	//		{
	//		case 0: // 차지카운터 0 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//			switch (m_iAttackCount)
	//			{
	//			case 0:
	//				//여기는 공격한번하면 카운트 1씩증가 
	//			{
	//				Get_OwnerCharacter().lock()->Change_State<CBatBossState_Storm>(0.05f);
	//			}
	//			break;	
	//			case 1:
	//			{
	//				int iRand = rand() % 2;
	//
	//				switch (iRand)
	//				{
	//				case 0:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Car>(0.05f);
	//					break;
	//				}
	//			}
	//				break;
	//			case 2:
	//			{
	//				int iRand = rand() % 2;
	//
	//				switch (iRand)
	//				{
	//				case 0:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Car>(0.05f);
	//					break;
	//				}
	//			}
	//				break;
	//			case 3:
	//			{
	//				Get_OwnerCharacter().lock()->Change_State<CBatBossState_Sp>(0.05f);
	//			}
	//				break;
	//			}			
	//			//차지 두번쨰일떄 
	//			break;
	//		}
	//		return true;
	//	}		
	//	else
	//	{
	//		if (m_bTurnCheck)
	//		{
	//			TurnMechanism();
	//		}
	//
	//		Get_OwnerCharacter().lock()->Change_State<CBatBossState_WalkF>(0.05f);
	//		return true;
	//	}
	//}
	//else
	//{
	//		if (fPToMDistance >= 8.f)
	//	{
	//		if (m_bTurnCheck)
	//		{
	//			TurnMechanism();
	//		}
	//
	//		switch (m_iChargeCount)
	//		{
	//		case 0: // 차지카운터 0 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//			switch (m_iAttackCount)
	//			{
	//			case 0:
	//				//여기는 공격한번하면 카운트 1씩증가 
	//			{
	//				int iRand = rand() % 2;
	//
	//				switch (iRand)
	//				{
	//				case 0:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Car>(0.05f);
	//					break;
	//				}
	//
	//			}
	//			break;
	//			case 1:
	//				// 공격한번더하면 카운트 또증가
	//			{
	//				int iRand = rand() % 2;
	//
	//				switch (iRand)
	//				{
	//				case 0:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//					break;
	//
	//				}
	//			}
	//			break;
	//			case 2:
	//				// 여기까지마지막임
	//			{
	//				int iRand = rand() % 2;
	//
	//				switch (iRand)
	//				{
	//				case 0:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//					break;
	//				}
	//			}
	//			break;
	//			case 3:
	//			{
	//				Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//			}
	//				break;
	//			}
	//			//차지한번도안했을때
	//			break;
	//		case 1: // 차지카운터가1일떄 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//			switch (m_iAttackCount)
	//			{
	//			case 0:
	//				//여기는 공격한번하면 카운트 1씩증가 
	//			{
	//				int iRand = rand() % 2;
	//
	//				switch (iRand)
	//				{
	//				case 0:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Car>(0.05f);
	//					break;
	//
	//				}
	//
	//			}
	//			break;
	//			case 1:
	//				// 공격한번더하면 카운트 또증가
	//			{
	//				int iRand = rand() % 2;
	//
	//				switch (iRand)
	//				{
	//				case 0:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//					break;
	//
	//				}
	//			}
	//			break;
	//			case 2:
	//				// 여기까지마지막임
	//			{
	//				int iRand = rand() % 2;
	//
	//				switch (iRand)
	//				{
	//				case 0:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//					break;
	//
	//				}
	//			}
	//			break;
	//			case 3:
	//			{
	//				Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//			}
	//				break;
	//			}
	//			//차지 한번햇을때
	//			break;
	//		}
	//
	//		return true;
	//
	//	     }
	//	      else
	//	      {
	//		       if (m_bTurnCheck)
	//		       {
	//			       TurnMechanism();
	//		       }
	//
	//			   Get_OwnerCharacter().lock()->Change_State<CBatBossState_WalkF>(0.05f);
	//			   return true;
    //          }
	//	
	//}



	return false;
}

