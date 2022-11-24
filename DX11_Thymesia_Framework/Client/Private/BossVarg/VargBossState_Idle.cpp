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



	//조건 8보다클때

	//보스존나어렵다 시발 ㅠㅠ ㅇㅇㅇ? ㅇㅇ 아 이거 랜덤으로 안하면 내가 정썜떄릴듯 ㄱㅊ? ㅇㅋㅇㅋ 최대한줄여봄 ㅇㅇ
	// 그럼 탈주함

	if (fPToMDistance < 1.f)
	{
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_WalkB>(0.05f);
		return true;
	}
    if (fPToMDistance > 5.f) // 5보다크다
	{
		int iRand = rand() % 2;

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_RunAttack>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_RunStart>(0.05f);
			break;
		}

		return true;
	}
	else  // 5보다 작다
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

