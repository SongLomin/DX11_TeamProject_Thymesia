#include "stdafx.h"
#include "BossBat/BatBossState_Stun_Start.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_Stun_Start);
CLONE_C(CBatBossState_Stun_Start, CComponent)

HRESULT CBatBossState_Stun_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Stun_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Stun_Start::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_StunStart");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Stun_Start::Call_AnimationEnd, this);
}

void CBatBossState_Stun_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Stun_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBatBossState_Stun_Start::OnStateStart(const _float& In_fAnimationBlendTime)
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


void CBatBossState_Stun_Start::OnStateEnd()
{
	__super::OnStateEnd();


}



void CBatBossState_Stun_Start::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Stun_Loop>(0.05f);
}

void CBatBossState_Stun_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Stun_Start::Call_AnimationEnd, this);
}

void CBatBossState_Stun_Start::Free()
{

}

_bool CBatBossState_Stun_Start::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

