#include "stdafx.h"
#include "BossUrd/UrdBossState_AttackComboC2.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "MobWeapon.h"
#include "Weapon.h"

GAMECLASS_C(CUrdBossState_AttackComboC2);
CLONE_C(CUrdBossState_AttackComboC2, CComponent)

HRESULT CUrdBossState_AttackComboC2::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_AttackComboC2::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_AttackComboC2::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Attack06|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_AttackComboC2::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_AttackComboC2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_AttackComboC2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_AttackComboC2::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
	for (auto& elem : pWeapons)
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NOPARRYATTACK, 1.1f);

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(4.f, 4.f, 4.f));

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_Attack(false);
	Get_Owner().lock()->Get_Component<CUrdBossState_Step_Idle>().lock()->Set_NoParryAttack(false);
	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_PhaseTwoSkillCount(1);

	m_bAttackLookAtLimit = true;
}	


void CUrdBossState_AttackComboC2::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));

}



void CUrdBossState_AttackComboC2::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	if (m_iAnimIndex != iEndAnimIndex)
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_AttackComboC2::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_AttackComboC2::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_AttackComboC2::Free()
{

}

_bool CUrdBossState_AttackComboC2::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();


	return false;
}

