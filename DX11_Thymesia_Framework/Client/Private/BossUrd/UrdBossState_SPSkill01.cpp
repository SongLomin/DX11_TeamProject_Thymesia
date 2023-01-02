#include "stdafx.h"
#include "BossUrd/UrdBossState_SPSkill01.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "MobWeapon.h"

GAMECLASS_C(CUrdBossState_SPSkill01);
CLONE_C(CUrdBossState_SPSkill01, CComponent)

HRESULT CUrdBossState_SPSkill01::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_SPSkill01::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_SPSkill01::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_SPSkill01|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_SPSkill01::Call_AnimationEnd, this);
}

void CUrdBossState_SPSkill01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_SPSkill01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_SPSkill01::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_SPSkill01::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_SPSkill01::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillCount(0);
	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SpecailAttack(false);
	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Equip_R>(0.05f);
}

void CUrdBossState_SPSkill01::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_SPSkill01::Call_AnimationEnd, this);
}

void CUrdBossState_SPSkill01::Free()
{

}

_bool CUrdBossState_SPSkill01::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	//weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	//list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();
	//
	//for (auto& elem : pWeapons)
	//{
	//	if (elem.lock()->Get_WeaponNum() == 1)
	//	{
	//		elem.lock()->Set_RenderOnOff(false);
	//	}
	//}

	return false;
}

