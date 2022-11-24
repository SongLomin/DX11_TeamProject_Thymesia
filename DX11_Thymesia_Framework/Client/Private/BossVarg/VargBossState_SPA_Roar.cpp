#include "stdafx.h"
#include "BossVarg/VargBossState_SPA_Roar.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_SPA_Roar);
CLONE_C(CVargBossState_SPA_Roar, CComponent)

HRESULT CVargBossState_SPA_Roar::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_SPA_Roar::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_SPA_Roar::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_SPAttack1_Roar");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_SPA_Roar::Call_AnimationEnd, this);*/
}

void CVargBossState_SPA_Roar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_SPA_Roar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_SPA_Roar::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: SPA_Roar -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_SPA_Roar::OnStateEnd()
{
	__super::OnStateEnd();


}


//
//void CVargBossState_SPA_Roar::Call_AnimationEnd()
//{
//	if (!Get_Enable())
//		return;
//
//
//	Get_OwnerCharacter().lock()->Change_State<CVargBossState_SPA_Roar>(0.05f);
//}

//void CVargBossState_SPA_Roar::OnDestroy()
//{
//	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_SPA_Roar::Call_AnimationEnd, this);
//}

void CVargBossState_SPA_Roar::Free()
{

}

_bool CVargBossState_SPA_Roar::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.1f)
	{
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_SPA_Roar>(0.05f);
		return true;
	}

	return false;
}

