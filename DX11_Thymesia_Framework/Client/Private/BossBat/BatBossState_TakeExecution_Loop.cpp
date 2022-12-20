#include "stdafx.h"
#include "BossBat/BatBossState_TakeExecution_Loop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_TakeExecution_Loop);
CLONE_C(CBatBossState_TakeExecution_Loop, CComponent)

HRESULT CBatBossState_TakeExecution_Loop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_TakeExecution_Loop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_TakeExecution_Loop::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|TAKELOOP");

}

void CBatBossState_TakeExecution_Loop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_TakeExecution_Loop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBatBossState_TakeExecution_Loop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif

}	


void CBatBossState_TakeExecution_Loop::OnStateEnd()
{
	__super::OnStateEnd();

}



void CBatBossState_TakeExecution_Loop::Free()
{

}

_bool CBatBossState_TakeExecution_Loop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	return false;
}

