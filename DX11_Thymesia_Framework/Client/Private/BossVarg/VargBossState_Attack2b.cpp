#include "stdafx.h"
#include "BossVarg/VargBossState_Attack2b.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_Attack2b);
CLONE_C(CVargBossState_Attack2b, CComponent)

HRESULT CVargBossState_Attack2b::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Attack2b::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Attack2b::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_ComboAttack2_2");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Attack2b::Call_AnimationEnd, this);
}

void CVargBossState_Attack2b::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Attack2b::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	Check_AndChangeNextState();
}



void CVargBossState_Attack2b::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;  // 애니메이션시작할떄 룩엣시작

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Attack2b -> OnStateStart" << endl;
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(2.f);
}

void CVargBossState_Attack2b::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);

}

void CVargBossState_Attack2b::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_Attack2b::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Attack2b::Call_AnimationEnd, this);
}

void CVargBossState_Attack2b::Free()
{

}

_bool CVargBossState_Attack2b::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.2f)
	{
		m_bAttackLookAtLimit = false;
	}



	return false;
}

