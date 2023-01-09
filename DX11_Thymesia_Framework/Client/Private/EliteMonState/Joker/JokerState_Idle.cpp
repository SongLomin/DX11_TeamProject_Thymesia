#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_Idle.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"



GAMECLASS_C(CJokerState_Idle);
CLONE_C(CJokerState_Idle, CComponent)

HRESULT CJokerState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_Idle::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_Idle");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_Idle::Call_AnimationEnd, this, placeholders::_1);*/
}

void CJokerState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_ComboA2>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_RunAtkEnd>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_JumpAttack>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_ShockAttack>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_StrongAttack>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_WheelAtkEnd>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_TurnAtkL>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_TurnAtkR>().lock())
	{
		m_bBackReset = false;
		m_bTurnCheck = true;
		
	}

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_WalkB>().lock())
	{
		m_bBackReset = true;
	}
	

	

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_Idle::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_Idle::Free()
{

}

_bool CJokerState_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리



	if (fPToMDistance <= 0.9f && !m_bBackReset)
	{
		if (m_bTurnCheck)
		{
			TurnMechanism();
		}
		
	
		Get_OwnerCharacter().lock()->Change_State<CJokerState_WalkB>(0.05f);
		return true;

		
	}
	else if (fPToMDistance < 4.f )
	{
		if (m_bTurnCheck)
		{
			TurnMechanism();
		}

		int iRand = rand() % 4;

		while (true)
		{
			if (iRand == m_iPreCount)
			{
				iRand = rand() % 5;
				continue;
			}
			else
			{
				break;
			}


		}

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_ComboA1>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_ShockAttack>(0.05f);
			break;
		case 2:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_StrongAttack>(0.05f);
			break;
		case 3:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_RunAttackStart>(0.05f);
			break;
		case 4:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_WheelAtkStart>(0.05f);
			break;
		}
		m_iPreCount = iRand;
		return true;
	}

	if (fPToMDistance > 4.f && fPToMDistance <= 7.f)
	{
		if (m_bTurnCheck)
		{
			TurnMechanism();
		}

		Get_OwnerCharacter().lock()->Change_State<CJokerState_WalkF>(0.05f);

		return true;
	}

	if (fPToMDistance > 7.f)
	{

		if (m_bTurnCheck)
		{
			TurnMechanism();
		}
		int iRand = rand() % 2;

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_JumpAttack>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_RunAttackStart>(0.05f);
			break;
		}
		

		return true;
	}

	return false;
}

