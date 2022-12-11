#include "stdafx.h"
#include "BossVarg/VargBossState_Attack2a.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "MobWeapon.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CVargBossState_Attack2a);
CLONE_C(CVargBossState_Attack2a, CComponent)

HRESULT CVargBossState_Attack2a::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Attack2a::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Attack2a::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_ComboAttack1_2");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Attack2a::Call_AnimationEnd, this);
}

void CVargBossState_Attack2a::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Attack2a::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
		TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_Attack2a::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bNextAttack = false;

	m_bAttackLookAtLimit = true;  // 애니메이션시작할떄 룩엣시작

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 150.f);
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit +=
		bind(&CVargBossState_Attack2a::Call_OtherControllerHit, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Attack2a -> OnStateStart" << endl;
#endif
#endif

	
}

void CVargBossState_Attack2a::OnStateEnd()
{
	__super::OnStateEnd();

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -=
		bind(&CVargBossState_Attack2a::Call_OtherControllerHit, this, placeholders::_1);
}



void CVargBossState_Attack2a::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_Attack2a::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Attack2a::Call_AnimationEnd, this);
}

void CVargBossState_Attack2a::Free()
{

}

_bool CVargBossState_Attack2a::Check_AndChangeNextState()
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


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f && !m_bNextAttack)
	{
		_uint iRand = rand() % 2;

		switch (iRand)
		{
		case 0:
			m_bNextAttack = true;
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack3a>(0.05f);
			break;
		}


		return true;
	}

	return false;
}

