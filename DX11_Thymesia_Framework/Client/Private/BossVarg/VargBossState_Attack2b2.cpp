#include "stdafx.h"
#include "BossVarg/VargBossState_Attack2b2.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_Attack2b2);
CLONE_C(CVargBossState_Attack2b2, CComponent)

HRESULT CVargBossState_Attack2b2::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Attack2b2::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Attack2b2::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_ComboAttack2_2b1");


	m_bAttackLookAtLimit = true;  // 애니메이션시작할떄 룩엣시작

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Attack2b2::Call_AnimationEnd, this);
}

void CVargBossState_Attack2b2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Attack2b2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
		Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_Attack2b2::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Attack2b2 -> OnStateStart" << endl;
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
}

void CVargBossState_Attack2b2::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}

void CVargBossState_Attack2b2::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_Attack2b2::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Attack2b2::Call_AnimationEnd, this);
}

void CVargBossState_Attack2b2::Free()
{

}

_bool CVargBossState_Attack2b2::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		m_bAttackLookAtLimit = false;
	}


	return false;
}

