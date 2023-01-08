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

	_float fPToMDistance = Get_DistanceWithPlayer(); // �÷��̾�� ���� �Ÿ�

	weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();


	if (KEY_INPUT(KEY::NUM2, KEY_STATE::TAP))
	{
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_VS_TakeExecution>(0.05f);
		return true;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() > 0 && m_bOnce)
	{
		m_pTransformCom.lock()->Set_Look2D(-m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK));
		m_bOnce = false;
	}

	//���⼭ ������Ѵ� 
	//�Ÿ��� 8���� ������ �������ϰ� 
	// ������ �ϰ� �ѹ��Ȱ� �ٽ� ���ǹ��� 

	

	if (m_bSpecailAttack)
	{
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_SPSkill01>(0.05f);
		return true;
		
	}
	else
	{
	    if (pStatus.lock()->Get_Desc().m_iLifeCount == 2)
		{
			if (fPToMDistance <= 4.3f)
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
						if (m_bSkillStart) // ���� �ǰ� ��������޸� ������ �̰Ÿ��������ϰԲ� �̷��� «
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
					if (m_bSkillStart) // ���� �ǰ� ��������޸� ������ �̰Ÿ��������ϰԲ� �̷��� «
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
							if (m_bAttack) // �Ÿ��� ������ �ջ��¿��� �ι� ��̰��������� ->�ٷ� �ߵ��ϰ�
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
			if (fPToMDistance <= 4.3f)
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
						if (m_bSkillStart) // ���� �ǰ� ��������޸� ������ �̰Ÿ��������ϰԲ� �̷��� «
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
					if (m_bSkillStart) // ���� �ǰ� ��������޸� ������ �̰Ÿ��������ϰԲ� �̷��� «
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
							if (m_bAttack) // �Ÿ��� ������ �ջ��¿��� �ι� ��̰��������� ->�ٷ� �ߵ��ϰ�
							{
								Get_OwnerCharacter().lock()->Change_State<CUrdBossState_AttackComboB1>(0.05f);
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

