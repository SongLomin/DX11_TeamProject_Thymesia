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
#include "State_Monster.h"

GAMECLASS_C(CUrdBossState_Attack_Idle);
CLONE_C(CUrdBossState_Attack_Idle, CComponent)

HRESULT CUrdBossState_Attack_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Attack_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Attack_Idle::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Idle|BaseLayer");

}

void CUrdBossState_Attack_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Attack_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Attack_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Get_Owner().lock()->Get_Component<CUrdBossState_Step_Idle>().lock()->Set_StepCloseCount(0);
	Get_Owner().lock()->Get_Component<CUrdBossState_Step_Idle>().lock()->Set_StepFarCount(0);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Attack_Idle::OnStateEnd()
{
	__super::OnStateEnd();

}




void CUrdBossState_Attack_Idle::Free()
{

}

_bool CUrdBossState_Attack_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();

	if (pStatus.lock()->Get_Desc().m_iLifeCount == 1)
	{
		if (m_bAttack)
		{
			int iRand = rand() % 3;


			if (m_iPreCount == iRand)
			{
				switch (m_iPreCount)
				{
				case 0:
					m_iPreCount = 1;
					break;
				case 1:
					m_iPreCount = 2;
					break;
				case 2:
					m_iPreCount = 0;
					break;
				default:
					break;
				}
			}
			else
			{
				m_iPreCount = iRand;
			}

			switch (m_iPreCount)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_AttackComboC1>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack05>(0.05f);
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack01LV2>(0.05f);
				break;
			}
			
			return true; 
		}
		else
		{
			int iRand = rand() % 2;


			if (m_iPreCount == iRand)
			{
				switch (m_iPreCount)
				{
				case 0:
					m_iPreCount = 1;
					break;
				case 1:
					m_iPreCount = 0;
					break;
				default:
					break;
				}
			}
			else
			{
				m_iPreCount = iRand;
			}

			switch (m_iPreCount)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_AttackComboB1>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_AttackComboC1>(0.05f);
				break;
			}
			
			return true;
		}
	}
	else // 1페이지일떄
	{
		// 공격패턴 -> 두번공격 찌르기세번 
		if (m_bAttack)
		{
			int iRand = rand() % 2;

			
			if (m_iPreCount == iRand)
			{
				switch (m_iPreCount)
				{
				case 0:
					m_iPreCount = 1;
					break;
				case 1:
					m_iPreCount = 0;
					break;
				default:
					break;
				}
			}
			else
			{
				m_iPreCount = iRand;
			}

		

			switch (m_iPreCount)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack05>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack01>(0.05f);
				break;
			}

			return true;
		}
		else
		{
			int iRand = rand() % 2;


			if (m_iPreCount == iRand)
			{
				switch (m_iPreCount)
				{
				case 0:
					m_iPreCount = 1;
					break;
				case 1:
					m_iPreCount = 0;
					break;
				default:
					break;
				}
			}
			else
			{
				m_iPreCount = iRand;
			}


			switch (m_iPreCount)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack05>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack01>(0.05f);
				break;
			}

			return true;
		}
	}


	return false;
}

void CUrdBossState_Attack_Idle::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		m_iPreCount = 5;

	}
}

