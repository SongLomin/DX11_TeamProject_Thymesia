#include "stdafx.h"
#include "BossBat/BatBossState_Stun_Loop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_Stun_Loop);
CLONE_C(CBatBossState_Stun_Loop, CComponent)

HRESULT CBatBossState_Stun_Loop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Stun_Loop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Stun_Loop::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_StunLoop");

	
}

void CBatBossState_Stun_Loop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Stun_Loop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBatBossState_Stun_Loop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pPhysXControllerCom.lock()->Enable_Gravity(true);
	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif


}	


void CBatBossState_Stun_Loop::OnStateEnd()
{
	__super::OnStateEnd();

}





void CBatBossState_Stun_Loop::Free()
{

}

_bool CBatBossState_Stun_Loop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

