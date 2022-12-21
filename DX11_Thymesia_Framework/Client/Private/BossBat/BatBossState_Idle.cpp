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

	if(fPToMDistance <= 10.f)
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_FTurnL>(0.05f);


	//차지상태 첫번쨰-> 두번쨰 ->헬모드\

	//if (m_bHeelScream) //헬모드일때
	//{
	//	if (fPToMDistance <= 11.f)
	//	{
	//		if (m_bTurnCheck)
	//		{
	//			TurnMechanism();
	//		}
	//		else
	//		{
	//			switch (m_iAttackCount)
	//			{
	//			case 0:
	//				Get_OwnerCharacter().lock()->Change_State<CBatBossState_Storm>(0.05f);
	//				break;
	//			case 1:
	//			{
	//				int iRand = rand() % 3;
	//				switch (iRand)
	//				{
	//				case 0:
	//					Check_CrossAttackState();
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Bite_1>(0.05f);
	//					break;
	//				case 2:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_SmarhL>(0.05f);
	//					break;
	//				}
	//			}
	//				break;
	//			case 2:
	//			{
	//				int iRand = rand() % 3;
	//				switch (iRand)
	//				{
	//				case 0:
	//					Check_CrossAttackState();
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Bite_1>(0.05f);
	//					break;
	//				case 2:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_SmarhL>(0.05f);
	//					break;
	//				}
	//			}
	//				break;
	//			case 3:
	//			{
	//				int iRand = rand() % 3;
	//				switch (iRand)
	//				{
	//				case 0:
	//					Check_CrossAttackState();
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Bite_1>(0.05f);
	//					break;
	//				case 2:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_SmarhL>(0.05f);
	//					break;
	//				}
	//			}
	//				break;
	//			case 4:
	//			{
	//				int iRand = rand() % 3;
	//				switch (iRand)
	//				{
	//				case 0:
	//					Check_CrossAttackState();
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Bite_1>(0.05f);
	//					break;
	//				case 2:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_SmarhL>(0.05f);
	//					break;
	//				}
	//			}
	//				
	//				break;
	//			case 5:
	//				Get_OwnerCharacter().lock()->Change_State<CBatBossState_Sp>(0.05f);
	//				break;
	//			}
	//		}
	//	}
	//	else
	//	{
	//
	//		if (m_bTurnCheck)
	//		{
	//			TurnMechanism();
	//		}
	//		else
	//		{
	//			switch (m_iAttackCount)
	//			{
	//			case 0:
	//				Get_OwnerCharacter().lock()->Change_State<CBatBossState_Storm>(0.05f);
	//				break;
	//			case 1:
	//			{
	//				int iRand = rand() % 2;
	//				switch (iRand)
	//				{
	//				case 0:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Car>(0.05f);
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//					break;
	//				}
	//			}
	//				break;
	//			case 2:
	//			{
	//				int iRand = rand() % 2;
	//				switch (iRand)
	//				{
	//				case 0:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Storm>(0.05f);
	//					break;
	//				case 1:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//					break;
	//				}
	//			}
	//				break;
	//			case 3:
	//				Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//				break;
	//			case 4:
	//				Get_OwnerCharacter().lock()->Change_State<CBatBossState_Sp>(0.05f);
	//				break;
	//			}
	//		}
	//	}
	//}
	//else //헬모드아닐떄
	//{
	//	if (fPToMDistance <= 11.f)
	//	{
	//		if (m_bTurnCheck)
	//		{
	//			TurnMechanism();
	//		}
	//		else
	//		{
	//			switch (m_iChargeCount)
	//			{
	//			case 0:
	//			{
	//				switch (m_iAttackCount)
	//				{
	//				case 0:
	//				{
	//					int iRand = rand() % 3;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Check_CrossAttackState();
	//						break;
	//					case 1:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Bite_1>(0.05f);
	//						break;
	//					case 2:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_SmarhL>(0.05f);
	//						break;
	//					}
	//				}
	//				break;
	//				case 1:
	//				{
	//					int iRand = rand() % 3;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Check_CrossAttackState();
	//						break;
	//					case 1:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Bite_1>(0.05f);
	//						break;
	//					case 2:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_SmarhL>(0.05f);
	//						break;
	//					}
	//				}
	//				break;
	//				case 2:
	//					Check_CrossAttackState();
	//					break;
	//				case 3:
	//				{
	//					int iRand = rand() % 2;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Storm>(0.05f);
	//						break;
	//					case 1:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//						break;
	//					}
	//				}
	//				break;
	//				case 4:							
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//					break;
	//				}
	//			}
	//			break;
	//			case 1: // 차지한번한상태 
	//			{
	//				switch (m_iAttackCount)
	//				{
	//				case 0:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_Chest>(0.05f);
	//					break;
	//				case 1:
	//				{
	//					int iRand = rand() % 2;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Check_CrossAttackState();
	//						break;
	//					case 1:
	//						if (m_bChestCheck)
	//						{
	//							Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_Chest>(0.05f);
	//						}
	//						else
	//						{
	//							Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_SmarhL>(0.05f);
	//						}
	//						break;
	//					}
	//				}
	//					break;
	//				case 2:
	//				{
	//					int iRand = rand() % 3;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Check_CrossAttackState();
	//						break;
	//					case 1:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Bite_1>(0.05f);
	//						break;
	//					case 2:
	//						if (m_bChestCheck)
	//						{
	//							Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_Chest>(0.05f);
	//						}
	//						else
	//						{
	//							Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_SmarhL>(0.05f);
	//						}				
	//						break;
	//					}
	//				}
	//					break;
	//				case 3:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Storm>(0.05f);
	//					break;
	//				case 4:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//					break;
	//				}
	//			}
	//			break;
	//			}
	//		}
	//		
	//	}
	//	else
	//	{
	//		if (m_bTurnCheck)
	//		{
	//			TurnMechanism();
	//		}
	//		else
	//		{
	//			switch (m_iChargeCount)
	//			{
	//			case 0:
	//			{
	//				switch (m_iAttackCount)
	//				{
	//				case 0:
	//				{
	//					int iRand = rand() % 2;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Car>(0.05f);
	//						break;
	//					case 1:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//						break;
	//					}
	//				}
	//					break;
	//				case 1:
	//				{
	//					int iRand = rand() % 2;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Car>(0.05f);
	//						break;
	//					case 1:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//						break;
	//					}
	//				}
	//					break;
	//				case 2:		
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Storm>(0.05f);
	//					break;							
	//				case 3:
	//				{
	//					int iRand = rand() % 2;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//						break;
	//					case 1:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//						break;
	//					}
	//				}
	//					break;
	//				case 4:
	//					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//					break;
	//				}
	//			}
	//			break;
	//			case 1: //어택카운터가 1일때
	//			{
	//				switch (m_iAttackCount)
	//				{
	//				case 0:
	//				{
	//					int iRand = rand() % 2;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Car>(0.05f);
	//						break;
	//					case 1:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//						break;
	//					}
	//				}
	//					break;
	//				case 1:
	//				{
	//					int iRand = rand() % 2;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Storm>(0.05f);
	//						break;
	//					case 1:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//						break;
	//					}
	//				}					
	//					break;
	//				case 2:
	//				{
	//					int iRand = rand() % 2;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Car>(0.05f);
	//						break;
	//					case 1:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//						break;
	//					}
	//				}
	//					break;
	//				case 3:
	//				{
	//					int iRand = rand() % 2;
	//					switch (iRand)
	//					{
	//					case 0:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_Storm>(0.05f);
	//						break;
	//					case 1:
	//						Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
	//						break;
	//					}
	//				}
	//					
	//				}
	//			    case 4:
	//				    Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
	//				    break;
	//			}
	//			break;
	//			}
	//		}
	//		}
	//		
	//	
	//}

	// 헬모드상태에서 -> 차지쓰면(sp)
	//
	// 10보다 가까우면  점프공격 



	



	return false;
}

