#include "stdafx.h"
#include "BossVarg/VargBossState_TurnAttack.h"
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

GAMECLASS_C(CVargBossState_TurnAttack);
CLONE_C(CVargBossState_TurnAttack, CComponent)

HRESULT CVargBossState_TurnAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_TurnAttack::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_TurnAttack::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TurnAttack1");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_TurnAttack::Call_AnimationEnd, this);
}

void CVargBossState_TurnAttack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_TurnAttack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();
}



void CVargBossState_TurnAttack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 150.f);
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(true);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: TurnAttack -> OnStateStart" << endl;
#endif
#endif

	
}

void CVargBossState_TurnAttack::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);

}



void CVargBossState_TurnAttack::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CVargBossState_Idle>().lock()->Set_TurnCheck(false);
	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_TurnAttack::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_TurnAttack::Call_AnimationEnd, this);
}

void CVargBossState_TurnAttack::Free()
{

}

_bool CVargBossState_TurnAttack::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.8f)
	{
		Get_Owner().lock()->Get_Component<CVargBossState_Idle>().lock()->Set_TurnCheck(false);
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
		return true;
	}

	return false;
}

