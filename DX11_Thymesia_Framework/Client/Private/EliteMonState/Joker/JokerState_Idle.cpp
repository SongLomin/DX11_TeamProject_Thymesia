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

	//���̳� �Ͼ��ÿ��� ���̵�� ������ ��ũ�� ������ 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_Idle");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_Idle::Call_AnimationEnd, this);*/
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
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_ComboB2>().lock()  ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_RunAtkEnd>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_JumpAttack>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_WheelAtkEnd>().lock())
	
	{
		m_bTurnCheck = true;
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

	_float fPToMDistance = Get_DistanceWithPlayer(); // �÷��̾�� ���� �Ÿ�
	_float fMToMDistance = GetStartPositionToCurrentPositionDir(); // ���ͽ�ŸƮ�����ǰ� �������� ������ ������ �Ÿ�



	if (fPToMDistance < 1.f)
	{
		int iRand = rand() % 3;

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_WalkB>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_WalkL>(0.05f);
			break;
		case 2:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_WalkR>(0.05f);
			break;
		}
		
		return true;
	
	}

	if (fPToMDistance >= 5.f)
	{
		if (m_bTurnCheck)
		{
			TurnMechanism();
		}
		else
		{
			int iRand = rand() % 3;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CJokerState_JumpAttack>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CJokerState_RunAttackStart>(0.05f);
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CJokerState_WalkF>(0.05f);
				break;
			}
		}


		return true;
	}

	if (fPToMDistance >= 1.f && fPToMDistance < 5.f)  // 5���� �۴�
	{
		if (m_bTurnCheck)
		{
			TurnMechanism();
		}
		else
		{
			int iRand = rand() % 5;
			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CJokerState_ComboA1>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CJokerState_ShockAttack>(0.05f);
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CJokerState_RunAttackStart>(0.05f);
				break;
			case 3:
				Get_OwnerCharacter().lock()->Change_State<CJokerState_WheelAtkStart>(0.05f);
				break;
			case 4:
				Get_OwnerCharacter().lock()->Change_State<CJokerState_Combob1>(0.05f);
				break;
			}
		}

		// 5���� ������ 1���� ������

		return true;
	}



	return false;
}

