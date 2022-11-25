#include "stdafx.h"
#include "BossVarg/VargBossState_RunAttack.h"
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

GAMECLASS_C(CVargBossState_RunAttack);
CLONE_C(CVargBossState_RunAttack, CComponent)

HRESULT CVargBossState_RunAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_RunAttack::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_RunAttack::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_RaidAttack1");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_RunAttack::Call_AnimationEnd, this);
}

void CVargBossState_RunAttack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);



	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_RunAttack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Rotation_TargetToLookDir();

	Check_AndChangeNextState();
}



void CVargBossState_RunAttack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 150.f);
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: RunAttack -> OnStateStart" << endl;
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(1.3f);
}

void CVargBossState_RunAttack::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}



void CVargBossState_RunAttack::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_RunAttack::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_RunAttack::Call_AnimationEnd, this);
}

void CVargBossState_RunAttack::Free()
{

}

_bool CVargBossState_RunAttack::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	return false;
}

