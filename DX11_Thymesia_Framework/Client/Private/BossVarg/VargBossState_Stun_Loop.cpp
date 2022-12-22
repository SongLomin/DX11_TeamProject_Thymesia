#include "stdafx.h"
#include "BossVarg/VargBossState_Stun_Loop.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_Stun_Loop);
CLONE_C(CVargBossState_Stun_Loop, CComponent)

HRESULT CVargBossState_Stun_Loop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Stun_Loop::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_StateFlag |= (_uint)STATE_FLAG::EXECUTABLE;

	return S_OK;
}

void CVargBossState_Stun_Loop::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_StunLoop");
}

void CVargBossState_Stun_Loop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Stun_Loop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Stun_Loop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: StunLoop -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Stun_Loop::OnStateEnd()
{
	__super::OnStateEnd();


}



void CVargBossState_Stun_Loop::Free()
{

}

_bool CVargBossState_Stun_Loop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	if (Get_OwnerMonster()->Get_BossExecutionStartOnOff())
	{
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_Exe_NoDeadEnd>(0.05f);
		return true;
	}


	return false;
}

