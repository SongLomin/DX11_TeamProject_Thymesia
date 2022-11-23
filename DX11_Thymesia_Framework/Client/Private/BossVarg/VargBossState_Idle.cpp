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
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
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

	if (fPToMDistance > 8.f)
	{
		int iRand = rand() % 3;

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

		return true;

		/*
		* 거리가 8보다크면 조건은 세가지
		* 1. 점프공격으로 거리줄이는방법 <점프두가지있음 존나크게점프임 이거는레이드어택>
		* 2. 달려오고 휘두로고 찍는 공격패턴 <런어택>
		* 3. 달려오면서 바로 공격 <런-> 어택->1 >
		*/
	}

	else if (4.f >= fPToMDistance || 8.f <= fPToMDistance)
	{
		int iRand = rand() % 5;

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack3a>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidL>(0.05f);
			break;
		case 2:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidR>(0.05f);
			break;
		case 3:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidB>(0.05f);
			break;
		case 4:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidAttack>(0.05f);
			break;
		}

		return true;
		/*
		거리가5보다크거나같고 8보자는작거나같을떄
		1. 찌르기 공격
		2. 양옆으로 갇다가 점프공격
		3. 그냥점프공격바로 ?이건생각좀
		*/
	}

	else if (4.f < fPToMDistance)
	{
		int iRand = rand() % 2;

		//턴하면 턴한다음 거기서 다음공격해야하고
		// 턴안하고 바로공격은 여기서처리할수있음

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack1a>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack2a>(0.05f);
			break;
		}

		return true;
		/*
		1. 보스몬스터보다 뒤에있으면 턴함 그리고 띵킹
		2. 보스몬스터보다 뒤에있으면 뒤로회전공격
		3. 공격패턴 1. 찍고 찍고 찌르기
		4. 공격패턴 2. 둥글이 다음에 찍기 ? 
		5. 공격패턴 3. 둥글이 찍기 3개있음 2-1 -> 2-2 , 2-1 -> 2-2B , 2-1->2-2B1 
		
		*/
	}


	return false;
}

