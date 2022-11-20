#include "stdafx.h"
#include "BossVarg/VargBossState_Exe_Dead.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_Stun_Exe_Dead);
CLONE_C(CVargBossState_Stun_Exe_Dead, CComponent)

HRESULT CVargBossState_Stun_Exe_Dead::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Stun_Exe_Dead::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Stun_Exe_Dead::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_Seq_TutorialBossFightStart");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Stun_Exe_Dead::Call_AnimationEnd, this);*/
}

void CVargBossState_Stun_Exe_Dead::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Stun_Exe_Dead::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Stun_Exe_Dead::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Stun_Exe_Dead::OnStateEnd()
{
	__super::OnStateEnd();


}


//
//void CVargBossState_Stun_Exe_Dead::Call_AnimationEnd()
//{
//	if (!Get_Enable())
//		return;
//
//
//	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Stun_Exe_Dead>(0.05f);
//}

//void CVargBossState_Stun_Exe_Dead::OnDestroy()
//{
//	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Stun_Exe_Dead::Call_AnimationEnd, this);
//}

void CVargBossState_Stun_Exe_Dead::Free()
{

}

_bool CVargBossState_Stun_Exe_Dead::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.1f)
	{
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_Stun_Exe_Dead>(0.05f);
		return true;
	}

	return false;
}

