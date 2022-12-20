#include "stdafx.h"
#include "BossBat/BatBossState_TakeExecution_End.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_TakeExecution_End);
CLONE_C(CBatBossState_TakeExecution_End, CComponent)

HRESULT CBatBossState_TakeExecution_End::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_TakeExecution_End::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_TakeExecution_End::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|TAKEEND");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_TakeExecution_End::Call_AnimationEnd, this);
}

void CBatBossState_TakeExecution_End::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_TakeExecution_End::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();
}



void CBatBossState_TakeExecution_End::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif

}	


void CBatBossState_TakeExecution_End::OnStateEnd()
{
	__super::OnStateEnd();

}



void CBatBossState_TakeExecution_End::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Sp>(0.05f);
}

void CBatBossState_TakeExecution_End::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_TakeExecution_End::Call_AnimationEnd, this);
}

void CBatBossState_TakeExecution_End::Free()
{

}

_bool CBatBossState_TakeExecution_End::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	return false;
}

