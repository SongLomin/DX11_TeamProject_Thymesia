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

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

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


	//여기서 해줘야한다 
	//거리가 8정도 가까우면 공격을하고 
	// 공격을 하고 한번걷고 다시 스탭밟음 
	if (fPToMDistance <= 8.f)
	{
		if (m_bSkillStart)
		{
			Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill_Idle>(0.05f);
			return true;
		}
		else
		{
			if (m_bAttack)
			{
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Walk_Idle>(0.05f);
				return true;
			}
			else
			{
				if (m_iStepCount == 0)
				{
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Step_Idle>(0.05f);
					return true;
				}
				else if (m_iStepCount == 1)
				{
					if (m_bThreeAttack)
					{
						//세번공격하거는거 여기서 바로 가도 될듯?
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack01>(0.05f);
						return true;
					}
					else
					{
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Step_Idle>(0.05f);
						return true;
					}
				}
				else
				{
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					return true;
				}
			}
		}
		//if (m_iStepCount == 1 && m_bThreeAttack)
		//{
		//	Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_ThreeAttack(true);
		//	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
		//	return true;
		//}
	}
	else
	{
		//멀엇을때
		// 거리가멀면 최대 3번 스탭을밟고
		//공격을하고
		//공격을한뒤에  한번걷고
		// 걸은다음에 또멀면 스탭을 최대 세번밟고 한번공격을하고 그다음에 특수공격을한다 그리고
		// 스킬카운트가 증가되어있으면 무조건 스킬발동이 먼저 스킬이 3일떄 특수패턴으로 바로들어감
		if (m_bSkillStart)
		{
			Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill_Idle>(0.05f);
			return true;
		}
		else
		{
			if (m_bAttack)
			{
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Walk_Idle>(0.05f);
				return true;
			}
			else
			{
				if (m_iStepCount == 0)
				{
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Step_Idle>(0.05f);
					return true;
				}
				else if (m_iStepCount == 1)
				{
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Step_Idle>(0.05f);
					return true;
				}
				else if (m_iStepCount == 2)
				{
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Step_Idle>(0.05f);
					return true;
				}
				else
				{
					//이때는 여기서 발 공격 가도 될듯?
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack02>(0.05f);
					return true;
				}
			}
			
		}

	}

	return false;
}

