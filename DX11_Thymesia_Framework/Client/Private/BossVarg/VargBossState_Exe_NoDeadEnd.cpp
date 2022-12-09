#include "stdafx.h"
#include "BossVarg/VargBossState_Exe_NoDeadEnd.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "Status_Boss.h"

GAMECLASS_C(CVargBossState_Exe_NoDeadEnd);
CLONE_C(CVargBossState_Exe_NoDeadEnd, CComponent)

HRESULT CVargBossState_Exe_NoDeadEnd::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Exe_NoDeadEnd::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Exe_NoDeadEnd::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TakeExecution_NoDeadEnd");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Exe_NoDeadEnd::Call_AnimationEnd, this);
}

void CVargBossState_Exe_NoDeadEnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Exe_NoDeadEnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Exe_NoDeadEnd::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Exe_SitDown -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Exe_NoDeadEnd::OnStateEnd()
{
	__super::OnStateEnd();


}



void CVargBossState_Exe_NoDeadEnd::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	m_pOwner.lock()->Get_Component<CStatus_Boss>().lock()->Set_NextPhase();
	GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::PLAYERHUD);
	
	Get_OwnerCharacter().lock()->Change_State<CVargBossState_SPA_Roar_Getup>(0.05f);
}

void CVargBossState_Exe_NoDeadEnd::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Exe_NoDeadEnd::Call_AnimationEnd, this);
}

void CVargBossState_Exe_NoDeadEnd::Free()
{

}

_bool CVargBossState_Exe_NoDeadEnd::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

