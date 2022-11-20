#include "stdafx.h"
#include "BossVarg/VargBossState_AvoidAttack.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_AvoidAttack);
CLONE_C(CVargBossState_AvoidAttack, CComponent)

HRESULT CVargBossState_AvoidAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_AvoidAttack::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_AvoidAttack::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_Seq_TutorialBossFightStart");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_AvoidAttack::Call_AnimationEnd, this);*/
}

void CVargBossState_AvoidAttack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_AvoidAttack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_AvoidAttack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_AvoidAttack::OnStateEnd()
{
	__super::OnStateEnd();


}


//
//void CVargBossState_AvoidAttack::Call_AnimationEnd()
//{
//	if (!Get_Enable())
//		return;
//
//
//	Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidAttack>(0.05f);
//}

//void CVargBossState_AvoidAttack::OnDestroy()
//{
//	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_AvoidAttack::Call_AnimationEnd, this);
//}

void CVargBossState_AvoidAttack::Free()
{

}

_bool CVargBossState_AvoidAttack::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.1f)
	{
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidAttack>(0.05f);
		return true;
	}

	return false;
}

