#include "stdafx.h"
#include "BossVarg/VargBossState_SPA_Run.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "Collider.h"

GAMECLASS_C(CVargBossState_SPA_Run);
CLONE_C(CVargBossState_SPA_Run, CComponent)

HRESULT CVargBossState_SPA_Run::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_SPA_Run::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_SPA_Run::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_SPAttack1_Run");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_SPA_Run::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_SPA_Run::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	Rotation_TargetToLookDir();

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_SPA_Run::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();
}



void CVargBossState_SPA_Run::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: SPA_Run -> OnStateStart" << endl;
#endif
#endif
}

void CVargBossState_SPA_Run::OnStateEnd()
{
	__super::OnStateEnd();
}

void CVargBossState_SPA_Run::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	if (m_iCount >= 1)
	{
		m_iCount = 0;
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_SPA_CatchFail>(0.05f);
	}
	else
	{
		m_iCount += 1;
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_SPA_Run>(0.05f);
	}

}



void CVargBossState_SPA_Run::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_SPA_Run::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_SPA_Run::Free()
{
}

_bool CVargBossState_SPA_Run::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

