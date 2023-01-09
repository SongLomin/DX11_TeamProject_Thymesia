#include "stdafx.h"
#include "BossVarg/VargBossState_Stun_Start.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "UI_ScriptQueue.h"




GAMECLASS_C(CVargBossState_Stun_Start);
CLONE_C(CVargBossState_Stun_Start, CComponent)

HRESULT CVargBossState_Stun_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Stun_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_StateFlag |= (_uint)STATE_FLAG::EXECUTABLE;

	return S_OK;
}

void CVargBossState_Stun_Start::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_StunStart");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Stun_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Stun_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Stun_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Stun_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	Get_OwnerMonster()->Set_RimLightDesc(6.f, { 0.5f,1.f,0.9f }, 1.f);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Sturn_Start -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Stun_Start::OnStateEnd()
{
	__super::OnStateEnd();

}



void CVargBossState_Stun_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Stun_Loop>(0.05f);
}

void CVargBossState_Stun_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Stun_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Stun_Start::Free()
{

}

_bool CVargBossState_Stun_Start::Check_AndChangeNextState()
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

