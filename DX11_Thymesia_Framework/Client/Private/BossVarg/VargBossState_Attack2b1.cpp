#include "stdafx.h"
#include "BossVarg/VargBossState_Attack2b1.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "../Public/BossVarg/Varg.h"
#include "VargWeapon.h"

GAMECLASS_C(CVargBossState_Attack2b1);
CLONE_C(CVargBossState_Attack2b1, CComponent)

HRESULT CVargBossState_Attack2b1::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Attack2b1::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Attack2b1::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_ComboAttack2_2b");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Attack2b1::Call_AnimationEnd, this);
}

void CVargBossState_Attack2b1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Attack2b1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
		TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_Attack2b1::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_bAttackLookAtLimit = true;  // 애니메이션시작할떄 룩엣시작

	Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(true);


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Attack2b1 -> OnStateStart" << endl;
#endif
#endif

	
}

void CVargBossState_Attack2b1::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);

}



void CVargBossState_Attack2b1::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_Attack2b1::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Attack2b1::Call_AnimationEnd, this);
}

void CVargBossState_Attack2b1::Free()
{

}

_bool CVargBossState_Attack2b1::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.3f)
	{
		m_bAttackLookAtLimit = false;
	}

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
	}


	return false;
}

