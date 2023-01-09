#include "stdafx.h"
#include "BossVarg/VargBossState_AvoidL.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_AvoidL);
CLONE_C(CVargBossState_AvoidL, CComponent)

HRESULT CVargBossState_AvoidL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_AvoidL::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_AvoidL::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_AvoidL");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_AvoidL::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_AvoidL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_AvoidL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Rotation_TargetToLookDir();

	Check_AndChangeNextState();
}



void CVargBossState_AvoidL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: AvoidL -> OnStateStart" << endl;
#endif
#endif
}

void CVargBossState_AvoidL::OnStateEnd()
{
	__super::OnStateEnd();
}



void CVargBossState_AvoidL::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

}

void CVargBossState_AvoidL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_AvoidL::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_AvoidL::Free()
{

}

_bool CVargBossState_AvoidL::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		int iRand = rand() % 2;

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidAttack>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack3a>(0.05f);
			break;
		}


		return true;
	}

	return false;
}

