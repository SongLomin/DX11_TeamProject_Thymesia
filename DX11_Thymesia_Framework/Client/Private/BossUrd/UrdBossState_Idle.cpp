#include "stdafx.h"
#include "BossUrd/UrdBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "Status_Boss.h"
#include "Status_Monster.h"

GAMECLASS_C(CUrdBossState_Idle);
CLONE_C(CUrdBossState_Idle, CComponent)

HRESULT CUrdBossState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Idle::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Idle|BaseLayer");

	
}

void CUrdBossState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CUrdBossState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_Attack01>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_Attack02>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_Attack05>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_VS_TakeExecution>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_Skill01>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_Skill02_1>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_Skill03_L>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_Skill03_R>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_SPSkill01>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_AttackComboB3>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_AttackComboC2>().lock())
	{
		m_bTurnCheck = true;
	}

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_VS_TakeExecution>().lock())
	{
		m_bOnce = true;
	}

	
	weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();

	_uint m_i = pStatus.lock()->Get_Desc().m_iLifeCount;

	if (pStatus.lock()->Get_Desc().m_iLifeCount == 1)
	{
		

		if (m_iPhaseTwoSkillCount >= 6)
		{
			m_bSkillStart = true;
			m_iPhaseTwoSkillCount = 0;
		}

		if (m_iPhaseTwoJavlinCount >= 3)
		{
			m_bSpecailAttack = true;
		}
	}
	
	

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex,0, In_fAnimationBlendTime);
}	


void CUrdBossState_Idle::OnStateEnd()
{
	__super::OnStateEnd();

}


void CUrdBossState_Idle::Free()
{

}

_bool CUrdBossState_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

	weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();


	//if (KEY_INPUT(KEY::NUM2, KEY_STATE::TAP))
	//{
	//	switch (m_iCount)
	//	{
	//	case 0:
	//		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_WalkF>(0.05f);
	//		break;
	//	case 1:
	//		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_WalkL>(0.05f);
	//		break;
	//	case 2:
	//		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_WalkR>(0.05f);
	//		break;
	//	case 3:
	//		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack02LV2C0>(0.05f);
	//		break;
	//	case 4:
	//		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack02LV2C1>(0.05f);
	//		break;
	//	case 5:
	//		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack05>(0.05f);
	//		break;
	//	case 6:
	//		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack06>(0.05f);
	//		break;
	//	case 7:
	//		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack07>(0.05f);
	//		break;
	//	case 8:
	//		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_AttackComboB1>(0.05f);
	//		break;
	//	case 9:
	//		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_AttackComboC1>(0.05f);
	//		break;
	//
	//	}
	//	return true;
	//}
	//
	//if (KEY_INPUT(KEY::NUM3, KEY_STATE::TAP))
	//{
	//	m_iCount += 1;
	//}
	//
	//if (KEY_INPUT(KEY::NUM4, KEY_STATE::TAP))
	//{
	//	m_iCount = 0;
	//}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() > 0 && m_bOnce)
	{
		m_pTransformCom.lock()->Set_Look2D(-m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK));
		m_bOnce = false;
	}

	//여기서 해줘야한다 
	//거리가 8정도 가까우면 공격을하고 
	// 공격을 하고 한번걷고 다시 스탭밟음 

	

	if (m_bSpecailAttack)
	{
		if (m_bTurnCheck)
		{			
			TurnMechanism();
			return true;
		}
		else
		{
			Get_OwnerCharacter().lock()->Change_State<CUrdBossState_SPSkill01>(0.05f);
			return true;
		}
	
		
	}
	else
	{
	    if (pStatus.lock()->Get_Desc().m_iLifeCount == 2)
		{
			if (fPToMDistance <= 4.5f)
			{
				if (m_bTurnCheck)
				{
					TurnMechanism();
				}
				else
				{
					if (m_bWalkStart)
					{
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Walk_Idle>(0.05f);
						return true;
					}
					else
					{
						if (m_bSkillStart) // 만약 피가 어느정도달면 무조건 이거먼저실행하게끔 이렇게 짬
						{
							Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill_Idle>(0.05f);
							return true;
						}
						else
						{
							Get_Owner().lock()->Get_Component<CUrdBossState_Step_Idle>().lock()->Set_StepClose(true);
							Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Step_Idle>(0.05f);
							return true;
						}
					}
	
				}
	
			}
			else
			{
				if (m_bTurnCheck)
				{
					TurnMechanism();
				}
				else
				{
					if (m_bSkillStart) // 만약 피가 어느정도달면 무조건 이거먼저실행하게끔 이렇게 짬
					{
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill_Idle>(0.05f);
						return true;
					}
					else
					{
						if (m_bWalkStart)
						{
							Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Walk_Idle>(0.05f);
							return true;
						}
						else
						{
							if (m_bAttack) // 거리가 존나게 먼상태에서 두번 댄싱공격했을떄 ->바로 발동하게
							{
								Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack02>(0.05f);
								return true;
							}
							else
							{
								Get_Owner().lock()->Get_Component<CUrdBossState_Step_Idle>().lock()->Set_StepClose(false);
								Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Step_Idle>(0.05f);
								return true;
							}
						}
	
					}
	
				}
	
			}
		}
		else if (pStatus.lock()->Get_Desc().m_iLifeCount == 1)
		{
			if (fPToMDistance <= 4.5f)
			{
				if (m_bTurnCheck)
				{
					TurnMechanism();
				}
				else
				{
					if (m_bWalkStart)
					{
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Walk_Idle>(0.05f);
						return true;
					}
					else
					{
						if (m_bSkillStart) // 만약 피가 어느정도달면 무조건 이거먼저실행하게끔 이렇게 짬
						{
							Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill_Idle>(0.05f);
							return true;
						}
						else
						{
							Get_Owner().lock()->Get_Component<CUrdBossState_Step_Idle>().lock()->Set_StepClose(true);
							Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Step_Idle>(0.05f);
							return true;
						}
					}
	
				}
	
			}
			else
			{
				if (m_bTurnCheck)
				{
					TurnMechanism();
				}
				else
				{
					if (m_bSkillStart) // 만약 피가 어느정도달면 무조건 이거먼저실행하게끔 이렇게 짬
					{
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill_Idle>(0.05f);
						return true;
					}
					else
					{
						if (m_bWalkStart)
						{
							Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Walk_Idle>(0.05f);
							return true;
						}
						else
						{
							if (m_bAttack) // 거리가 존나게 먼상태에서 두번 댄싱공격했을떄 ->바로 발동하게
							{
								Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack07>(0.05f);
								return true;
							}
							else
							{
								Get_Owner().lock()->Get_Component<CUrdBossState_Step_Idle>().lock()->Set_StepClose(false);
								Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Step_Idle>(0.05f);
								return true;
							}
						}
	
					}
	
				}
	
			}
		}
		
	}
	

	return false;
}

void CUrdBossState_Idle::OnEventMessage(_uint iArg)
{

	__super::OnEventMessage(iArg);


	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		m_bSkillStart = false;
		m_bTurnCheck = false;
		m_bWalkStart = false;
		m_bSpecailAttack = false;
		m_bAttack = false;
		m_iPhaseTwoJavlinCount = 0;
		m_iPhaseTwoSkillCount = 0;
		m_iSkillCount = 0;
	}
}

