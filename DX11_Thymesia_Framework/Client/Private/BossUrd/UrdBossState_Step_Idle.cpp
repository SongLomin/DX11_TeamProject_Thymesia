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


GAMECLASS_C(CUrdBossState_Step_Idle);
CLONE_C(CUrdBossState_Step_Idle, CComponent)

HRESULT CUrdBossState_Step_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Step_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Step_Idle::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Idle|BaseLayer");

}

void CUrdBossState_Step_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Step_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Step_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Step_Idle::OnStateEnd()
{
	__super::OnStateEnd();

}


void CUrdBossState_Step_Idle::Free()
{

}

_bool CUrdBossState_Step_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;
	
	// step FL,FL56 FR,FR45 DANCINGF,DANCINGR
	

	weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();

	if (pStatus.lock()->Get_Desc().m_iLifeCount == 2)
	{
		if (m_bStepClose) //가까울떄
		{
			if (m_iStepCloseCount == 0) //스탭아이들에서 -> 어택아이들로 넘어가는게 맞는듯?
			{
				int iRand = rand() % 5;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepR>(0.05f);
					m_iStepCloseCount = 1;
					m_iStepFarCount = 2;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepL>(0.05f);
					m_iStepCloseCount = 2;
					m_iStepFarCount = 1;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR>(0.05f);
					m_iStepCloseCount = 3;
					break;
				case 3:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL>(0.05f);
					m_iStepCloseCount = 4;
					break;
				
				case 4:
				{
					int iRand = rand() % 2;
					switch (iRand)
					{
					case 0:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					case 1:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					}
				}
					break;
				}
				return true;
			
			}
			else if (m_iStepCloseCount == 1) //오른쪽
			{
				//둘중에하나 왼쪽공격 또는 왼쪽
				int iRand = rand() % 2;
				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_L>(0.05f);
					break;
				case 1:
					int iRand = rand() % 2;
					switch (iRand)
					{
					case 0:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					case 1:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					}
					break;
				}
				return true;
			}
			else if (m_iStepCloseCount == 2) //왼쪽
			{
				int iRand = rand() % 2;
				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_R>(0.05f);
					break;
				case 1:
					int iRand = rand() % 2;
					switch (iRand)
					{
					case 0:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					case 1:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					}
					break;
				}
				return true;
			}
			else if (m_iStepCloseCount == 3) //오른쪽앞
			{
				int iRand = rand() % 3;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepL>(0.05f);
					m_iStepCloseCount = 5;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_L>(0.05f);
					break;
				case 2:
					int iRand = rand() % 2;
					switch (iRand)
					{
					case 0:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					case 1:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					}
					break;
				}
				return true;
			}
			else if (m_iStepCloseCount == 4) //왼쪽앞
			{
				int iRand = rand() % 3;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepR>(0.05f);
					m_iStepCloseCount = 6;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_R>(0.05f);
					break;
				case 2:
					int iRand = rand() % 2;
					switch (iRand)
					{
					case 0:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					case 1:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					}
					break;
				}
				return true;
			}
			else if (m_iStepCloseCount == 5) // 왼쪽
			{
				int iRand = rand() % 2;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_R>(0.05f);
					break;
				case 1:
					int iRand = rand() % 2;
					switch (iRand)
					{
					case 0:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					case 1:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					}
					break;
				}
				return true;
			}
			else if (m_iStepCloseCount == 6) // 오른쪽
			{
				int iRand = rand() % 2;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_L>(0.05f);
					break;
				case 1:
					int iRand = rand() % 2;
					switch (iRand)
					{
					case 0:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					case 1:
						Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
						Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
						break;
					}
					break;
				}

				return true;
			}




		}
		else //디폴트값 멀떄
		{
			if (m_iStepFarCount == 0) //
			{
				int iRand = rand() % 4;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR>(0.05f);
					m_iStepCloseCount = 6;
					m_iStepFarCount = 2;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR45>(0.05f);
					m_iStepCloseCount = 6;
					m_iStepFarCount = 2;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL>(0.05f);
					m_iStepCloseCount = 5;
					m_iStepFarCount = 1;
					break;
				case 3:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL45>(0.05f);
					m_iStepCloseCount = 5;
					m_iStepFarCount = 1;
					break;
				}
				return true;
			}
			else if (m_iStepFarCount == 1) // 스탭이 외쪽일때 -> 오른쪽으로
			{
				int iRand = rand() % 3;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR>(0.05f);
					m_iStepCloseCount = 6;
					m_iStepFarCount = 4;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR45>(0.05f);
					m_iStepCloseCount = 6;
					m_iStepFarCount = 4;
					break;
				}
				return true;
			}
			else if (m_iStepFarCount == 2)  //스탭이 오른쪽일떄 -> 왼쪽으로
			{
				int iRand = rand() % 3;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL>(0.05f);
					m_iStepCloseCount = 5;
					m_iStepFarCount = 3;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL45>(0.05f);
					m_iStepCloseCount = 5;
					m_iStepFarCount = 3;
					break;
				}
				return true;
			}
			else if (m_iStepFarCount == 3) //  스탭이 왼쪽일떄
			{
				if (m_bNoParryAttack)
				{
					// 이떄 아이들한테 다음공격떄 파란색공격하게 하며될듯?초기화	
					Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_Attack(true);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack02>(0.05f);
					return true;
				}
				else
				{
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_R>(0.05f);
					m_bNoParryAttack = true;
					return true;
				}

			}
			else if (m_iStepFarCount == 4) // 스탭이 오른쪽일때
			{
				if (m_bNoParryAttack)
				{
					// 이떄 아이들한테 다음공격떄 파란색공격하게 하며될듯? 이떄초기화해줘야함
					Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_Attack(true);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack02>(0.05f);
					return true;
				}
				else
				{
					//이떄초기화해줘야함 어서함? 대쉬에서함
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_L>(0.05f);
					m_bNoParryAttack = true;
					return true;
				}
			}

		}
	}
	else // 이떄가 그거임///////////////////////////////////////////////////////////////////////////////////////
	{
	if (m_bStepClose) //가까울떄
	{
		if (m_iStepCloseCount == 0) //스탭아이들에서 -> 어택아이들로 넘어가는게 맞는듯?
		{
			int iRand = rand() % 5;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepR>(0.05f);
				m_iStepCloseCount = 1;
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepL>(0.05f);
				m_iStepCloseCount = 2;
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR>(0.05f);
				m_iStepCloseCount = 3;
				break;
			case 3:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL>(0.05f);
				m_iStepCloseCount = 4;
				break;
			case 4:
			{
				int iRand = rand() % 2;
				switch (iRand)
				{
				case 0:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				case 1:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				}
			}
				
				break;
			}
			return true;
		}
		else if (m_iStepCloseCount == 1) //오른쪽
		{
			//둘중에하나 왼쪽공격 또는 왼쪽
			int iRand = rand() % 2;
			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_L>(0.05f);
				break;

			case 1:
			{
				int iRand = rand() % 2;
				switch (iRand)
				{
				case 0:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				case 1:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				}
			}
				
				break;
			}
			return true;
		}
		else if (m_iStepCloseCount == 2) //왼쪽
		{
			int iRand = rand() % 2;
			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_R>(0.05f);
				break;
			case 1:
			{

				int iRand = rand() % 2;
				switch (iRand)
				{
				case 0:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				case 1:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				}
			}
				break;
			}
			return true;
		}
		else if (m_iStepCloseCount == 3) //오른쪽앞
		{
			int iRand = rand() % 3;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepL>(0.05f);
				m_iStepCloseCount = 5;
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_L>(0.05f);
				break;
			case 2:
			{
				int iRand = rand() % 2;
				switch (iRand)
				{
				case 0:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				case 1:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				}
			}
				
				break;
			}
			return true;
		}
		else if (m_iStepCloseCount == 4) //왼쪽앞
		{
			int iRand = rand() % 3;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepR>(0.05f);
				m_iStepCloseCount = 6;
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_R>(0.05f);
				break;
			case 2:
			{
				int iRand = rand() % 2;
				switch (iRand)
				{
				case 0:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				case 1:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				}
			}
				
				break;
			}
			return true;
		}
		else if (m_iStepCloseCount == 5) // 왼쪽
		{
			int iRand = rand() % 2;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_R>(0.05f);
				break;
			case 1:
			{
				int iRand = rand() % 2;
				switch (iRand)
				{
				case 0:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				case 1:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				}
			}
				
				break;
			}
			return true;
		}
		else if (m_iStepCloseCount == 6) // 오른쪽
		{
			int iRand = rand() % 2;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_L>(0.05f);
				break;
			case 1:
			{
				int iRand = rand() % 2;
				switch (iRand)
				{
				case 0:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(true);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				case 1:
					Get_Owner().lock()->Get_Component<CUrdBossState_Attack_Idle>().lock()->Set_Attack(false);
					Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack_Idle>(0.05f);
					break;
				}
			}
				
				break;
			}

			return true;
		}




	}
	else //디폴트값 멀떄
	{
		if (m_iStepFarCount == 0) //
		{
			int iRand = rand() % 4;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR>(0.05f);
				m_iStepCloseCount = 6;
				m_iStepFarCount = 2;
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR45>(0.05f);
				m_iStepCloseCount = 6;
				m_iStepFarCount = 2;
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL>(0.05f);
				m_iStepCloseCount = 5;
				m_iStepFarCount = 1;
				break;
			case 3:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL45>(0.05f);
				m_iStepCloseCount = 5;
				m_iStepFarCount = 1;
				break;
			}
			return true;
		}
		else if (m_iStepFarCount == 1) // 스탭이 외쪽일때 -> 오른쪽으로
		{
			int iRand = rand() % 3;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR>(0.05f);
				m_iStepCloseCount = 6;
				m_iStepFarCount = 4;
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR45>(0.05f);
				m_iStepCloseCount = 6;
				m_iStepFarCount = 4;
				break;
			}
			return true;
		}
		else if (m_iStepFarCount == 2)  //스탭이 오른쪽일떄 -> 왼쪽으로
		{
			int iRand = rand() % 3;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL>(0.05f);
				m_iStepCloseCount = 5;
				m_iStepFarCount = 3;
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL45>(0.05f);
				m_iStepCloseCount = 5;
				m_iStepFarCount = 3;
				break;
			}
			return true;
		}
		else if (m_iStepFarCount == 3) //  스탭이 왼쪽일떄
		{
			if (m_bNoParryAttack)
			{
				// 이떄 아이들한테 다음공격떄 파란색공격하게 하며될듯?초기화	
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_Attack(true);
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_AttackComboC1>(0.05f);
				return true;
			}
			else
			{
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_R>(0.05f);
				m_bNoParryAttack = true;
				return true;
			}

		}
		else if (m_iStepFarCount == 4) // 스탭이 오른쪽일때
		{
			if (m_bNoParryAttack)
			{
				// 이떄 아이들한테 다음공격떄 파란색공격하게 하며될듯? 이떄초기화해줘야함
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_Attack(true);
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_AttackComboB1>(0.05f);
				return true;
			}
			else
			{
				//이떄초기화해줘야함 어서함? 대쉬에서함
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_L>(0.05f);
				m_bNoParryAttack = true;
				return true;
			}
		}

	}
	}



	
	
	return false;
}

void CUrdBossState_Step_Idle::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		m_iStepFarCount = 0;
		m_iStepCloseCount = 0;
		m_bNoParryAttack = false;
		m_bStepClose = false;

	}
}

