#include "stdafx.h"
#include "BossVarg/VargBossState_AvoidB.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_AvoidB);
CLONE_C(CVargBossState_AvoidB, CComponent)

HRESULT CVargBossState_AvoidB::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_AvoidB::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_AvoidB::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_AvoidB");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_AvoidB::Call_AnimationEnd, this);
}

void CVargBossState_AvoidB::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Rotation_TargetToLookDir();

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_AvoidB::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_AvoidB::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: AvoidB -> OnStateStart" << endl;
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);

}

void CVargBossState_AvoidB::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}



void CVargBossState_AvoidB::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidAttack>(0.05f);
}

void CVargBossState_AvoidB::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_AvoidB::Call_AnimationEnd, this);
}

void CVargBossState_AvoidB::Free()
{

}

_bool CVargBossState_AvoidB::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidAttack>(0.05f);

		return true;
	}


	return false;
}

