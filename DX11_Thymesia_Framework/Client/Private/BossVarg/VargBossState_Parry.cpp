#include "stdafx.h"
#include "BossVarg/VargBossState_Parry.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_Parry);
CLONE_C(CVargBossState_Parry, CComponent)

HRESULT CVargBossState_Parry::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Parry::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Parry::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TakeParry_FIX");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Parry::Call_AnimationEnd, this, placeholders::_1);*/
}

void CVargBossState_Parry::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Parry::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Parry::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Parry -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Parry::OnStateEnd()
{
	__super::OnStateEnd();


}


//
//void CVargBossState_Parry::Call_AnimationEnd(_uint iEndAnimIndex)
//{
//	if (!Get_Enable())
//		return;
//
//
//	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Parry>(0.05f);
//}

//void CVargBossState_Parry::OnDestroy()
//{
//	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Parry::Call_AnimationEnd, this, placeholders::_1);
//}

void CVargBossState_Parry::Free()
{

}

_bool CVargBossState_Parry::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	return false;
}

