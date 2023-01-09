#include "stdafx.h"
#include "BossVarg/VargBossState_Exe_SitLoop.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_Exe_SitLoop);
CLONE_C(CVargBossState_Exe_SitLoop, CComponent)

HRESULT CVargBossState_Exe_SitLoop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Exe_SitLoop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Exe_SitLoop::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TakeExecution_Loop");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Exe_SitLoop::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Exe_SitLoop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Exe_SitLoop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Exe_SitLoop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Exe_SitLoop -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Exe_SitLoop::OnStateEnd()
{
	__super::OnStateEnd();


}



void CVargBossState_Exe_SitLoop::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Exe_Dead>(0.05f);
}

void CVargBossState_Exe_SitLoop::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Exe_SitLoop::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Exe_SitLoop::Free()
{

}

_bool CVargBossState_Exe_SitLoop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	return false;
}

