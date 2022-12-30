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

	_float fPToMDistance = Get_DistanceWithPlayer(); // �÷��̾�� ���� �Ÿ�


	//���⼭ ������Ѵ� 
	//�Ÿ��� 8���� ������ �������ϰ� 
	// ������ �ϰ� �ѹ��Ȱ� �ٽ� ���ǹ��� 
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
						//���������ϰŴ°� ���⼭ �ٷ� ���� �ɵ�?
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
		//�־�����
		// �Ÿ����ָ� �ִ� 3�� ���������
		//�������ϰ�
		//�������ѵڿ�  �ѹ��Ȱ�
		// ���������� �Ǹָ� ������ �ִ� ������� �ѹ��������ϰ� �״����� Ư���������Ѵ� �׸���
		// ��ųī��Ʈ�� �����Ǿ������� ������ ��ų�ߵ��� ���� ��ų�� 3�ϋ� Ư���������� �ٷε�
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
					//�̶��� ���⼭ �� ���� ���� �ɵ�?
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack02>(0.05f);
					return true;
				}
			}
			
		}

	}

	return false;
}

