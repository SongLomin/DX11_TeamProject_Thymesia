#include "stdafx.h"
#include "BossBat/BatBossState_Start_Loop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CBatBossState_Start_Loop);
CLONE_C(CBatBossState_Start_Loop, CComponent)

HRESULT CBatBossState_Start_Loop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Start_Loop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Start_Loop::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_Seq_BossFightStart_Loop");

	//m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Start_Loop::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Start_Loop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Start_Loop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CBatBossState_Start_Loop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif

}	

void CBatBossState_Start_Loop::OnStateEnd()
{
	__super::OnStateEnd();

}

void CBatBossState_Start_Loop::Free()
{

}

_bool CBatBossState_Start_Loop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

	
	if (fPToMDistance <= 30.f)
	{
		Get_OwnerCharacter().lock()->Change_State<CBatBossState_Start>(0.05f);
		return true;
	}

	return false;
}

