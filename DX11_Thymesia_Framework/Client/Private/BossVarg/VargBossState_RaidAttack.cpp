#include "stdafx.h"
#include "BossVarg/VargBossState_RaidAttack.h"
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
#include "MobWeapon.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CVargBossState_RaidAttack);
CLONE_C(CVargBossState_RaidAttack, CComponent)

HRESULT CVargBossState_RaidAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_RaidAttack::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_RaidAttack::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_RaidAttack2");

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_RaidAttack::Call_AnimationEnd, this);
}

void CVargBossState_RaidAttack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAttackLookAtLimit)
		Turn_ToThePlayer(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_RaidAttack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(m_bAttackLookAtLimit)
	Rotation_TargetToLookDir();


	Check_AndChangeNextState();
}



void CVargBossState_RaidAttack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(true);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit +=
		bind(&CVargBossState_RaidAttack::Call_OtherControllerHit, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: RaidAttack -> OnStateStart" << endl;
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(2.f);
}

void CVargBossState_RaidAttack::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
	Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);


	m_pPhysXControllerCom.lock()->Callback_ControllerHit -=
		bind(&CVargBossState_RaidAttack::Call_OtherControllerHit, this, placeholders::_1);
}



void CVargBossState_RaidAttack::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_RaidAttack::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_RaidAttack::Call_AnimationEnd, this);
}

void CVargBossState_RaidAttack::Free()
{

}

_bool CVargBossState_RaidAttack::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.2f)
	{
		m_bAttackLookAtLimit = false;
	}


	return false;
}

