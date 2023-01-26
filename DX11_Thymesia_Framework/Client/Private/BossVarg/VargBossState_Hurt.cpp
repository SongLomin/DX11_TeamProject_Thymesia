#include "stdafx.h"
#include "BossVarg/VargBossState_Hurt.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_Hurt);
CLONE_C(CVargBossState_Hurt, CComponent)

HRESULT CVargBossState_Hurt::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Hurt::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Hurt::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_HurtF");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Hurt::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Hurt::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Hurt::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Hurt::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Hurt -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Hurt::OnStateEnd()
{
	__super::OnStateEnd();


}



void CVargBossState_Hurt::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_Hurt::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Hurt::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Hurt::Free()
{

}

_bool CVargBossState_Hurt::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	return false;
}

