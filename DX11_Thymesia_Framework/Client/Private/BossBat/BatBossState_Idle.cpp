#include "stdafx.h"
#include "BossBat/BatBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_Idle);
CLONE_C(CBatBossState_Idle, CComponent)

HRESULT CBatBossState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Idle::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|BossBat_Idle");

	//m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Idle::Call_AnimationEnd, this);
}

void CBatBossState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CBatBossState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_Idle::OnStateEnd()
{
	__super::OnStateEnd();



}



//void CBatBossState_Idle::Call_AnimationEnd()
//{
//	if (!Get_Enable())
//		return;
//
//	Get_OwnerCharacter().lock()->Change_State<CVargBossState_WalkF>(0.05f);
//}
//
//void CBatBossState_Idle::OnDestroy()
//{
//	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Idle::Call_AnimationEnd, this);
//}

void CBatBossState_Idle::Free()
{

}

_bool CBatBossState_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;





	return false;
}

