#include "stdafx.h"
#include "BossVarg/VargBossState_Exe_End.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_Stun_Exe_End);
CLONE_C(CVargBossState_Stun_Exe_End, CComponent)

HRESULT CVargBossState_Stun_Exe_End::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Stun_Exe_End::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Stun_Exe_End::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TakeExecution_Loop");


	
}

void CVargBossState_Stun_Exe_End::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Stun_Exe_End::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Stun_Exe_End::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Exe_End -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Stun_Exe_End::OnStateEnd()
{
	__super::OnStateEnd();


}







void CVargBossState_Stun_Exe_End::Free()
{

}

_bool CVargBossState_Stun_Exe_End::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

