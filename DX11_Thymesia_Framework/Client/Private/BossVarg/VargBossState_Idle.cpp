#include "stdafx.h"
#include "BossVarg/VargBossState_Idle.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"



GAMECLASS_C(CVargBossState_Idle);
CLONE_C(CVargBossState_Idle, CComponent)

HRESULT CVargBossState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Idle::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 




	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_Idle");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Idle::Call_AnimationEnd, this);*/
}

void CVargBossState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack3a>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack2b>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_RunAttack>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack2b1>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack2b2>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack2a>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_AvoidAttack>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_RaidAttack>().lock())
	{

		m_bTurnCheck = true;
		
	}

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack3a>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_AvoidAttack>().lock())

	{
		m_iBackCount = 1;
	}

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack2b>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_RunAttack>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack2b1>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack2b2>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack2a>().lock()  ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_RaidAttack>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Attack3b>().lock())
	{

		m_iBackCount = 0;

	}


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Idle::OnStateEnd()
{
	__super::OnStateEnd();


}

void CVargBossState_Idle::Free()
{

}

_bool CVargBossState_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

	DOT_RESULT DotReuslt = DOT_RESULT::DOT_RESULT_END;

	
	
	if (fPToMDistance >= 0.8f && fPToMDistance < 1.5f &&  m_iBackCount == 0)
	{
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_WalkB>(0.05f);
		return true;
		

	}
	if (fPToMDistance >= 7.f) // 5보다크다
	{
		int iRand = rand() % 3;

		if (m_bTurnCheck)
		{
			TurnMechanism();
		}
		else
		{
			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CVargBossState_RaidAttack>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CVargBossState_RunAttack>(0.05f);
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CVargBossState_RunStart>(0.05f);
				break;
			}
		}

		return true;
	}
	if (fPToMDistance >= 0.8f && fPToMDistance < 7.f)  // 5보다 작다
	{
		if (m_bTurnCheck)
		{
			TurnMechanism();
		}
		else
		{
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_WalkF>(0.05f);
		}

		// 5보다 작을떄 1보다 작을떄

		return true;
	}






	return false;
}

