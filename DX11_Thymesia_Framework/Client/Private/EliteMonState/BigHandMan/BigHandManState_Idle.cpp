#include "stdafx.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"



GAMECLASS_C(CBigHandManState_Idle);
CLONE_C(CBigHandManState_Idle, CComponent)

HRESULT CBigHandManState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBigHandManState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBigHandManState_Idle::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV2Villager_M.ao|LV2Villager01_M_Idle");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBigHandManState_Idle::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBigHandManState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBigHandManState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBigHandManState_ComboA01>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBigHandManState_ComboA02>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBigHandManState_ComboB_End>().lock())
	{
		m_bTurnCheck = true;
	}
	

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "BigHandManState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CBigHandManState_Idle::OnStateEnd()
{
	__super::OnStateEnd();


}

void CBigHandManState_Idle::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	
}

void CBigHandManState_Idle::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBigHandManState_Idle::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_Idle::Free()
{

}

_bool CBigHandManState_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

	//아이들에서 거리가 가까울떄 멀때 중간일때 다달름 

	if (m_bFightStart)
	{
		if (m_bTurnCheck)
		{
			TurnMechanism();
		}
		else
		{
			if (fPToMDistance <= 4.f)
			{

				if (m_bComboAttackRooting)
				{
					Get_Owner().lock()->Get_Component<CBigHandManState_ComboB_Start>().lock()->Set_FourAttackEnd(true);
					Get_OwnerCharacter().lock()->Change_State<CBigHandManState_ComboB_Start>(0.05f);
					m_bComboAttackRooting = false;
					return true;
				}
				else
				{
					int iRand = rand() % 2;

					switch (iRand)
					{
					case 1:
						//만약한번공격했으면 다음공격에서 거리가까우면 무조건 콤보공격으로 가게끔
						Get_OwnerCharacter().lock()->Change_State<CBigHandManState_ComboA01>(0.05f);
						m_bComboAttackRooting = true;
						break;
					case 0:
						//이떄는거리가 가깝기떄문에 4번치고 점프공격
						Get_Owner().lock()->Get_Component<CBigHandManState_ComboB_Start>().lock()->Set_FourAttackEnd(true);
						Get_OwnerCharacter().lock()->Change_State<CBigHandManState_ComboB_Start>(0.05f);
						break;
					}
				}


				return true;
			}

			if (fPToMDistance > 4.f && fPToMDistance <= 8.f)
			{
				//이떄는 한번점프치고  바로 애드로 가는거임
				Get_Owner().lock()->Get_Component<CBigHandManState_ComboB_Start>().lock()->Set_OneAttackEnd(true);
				Get_OwnerCharacter().lock()->Change_State<CBigHandManState_ComboB_Start>(0.05f);
				return true;
			}
			if (fPToMDistance > 8.f)
			{
				// 이때는 점프스타트이후 -> 쫓아오고 거리가일정거리가까워지면 앤드로가게끔
				Get_Owner().lock()->Get_Component<CBigHandManState_ComboB_Start>().lock()->Set_OneAttackRun(true);
				Get_OwnerCharacter().lock()->Change_State<CBigHandManState_ComboB_Start>(0.05f);
				return true;
			}
		}

	}
	else
	{
		if (fPToMDistance <= 4.f && !m_bFightStart)
		{
			m_bFightStart = true;
			m_bTurnCheck = true;
		}
	}
	
	


	return false;
}

