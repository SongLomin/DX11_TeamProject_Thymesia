#include "stdafx.h"
#include "BossUrd/UrdBossState_Attack03_DashSting_L.h"
#include "BossUrd/UrdBossState_Step_Idle.h"
#include "BossUrd/UrdBossState_Idle.h"
#include "MobWeapon.h"
#include "Monster.h"
#include "Animation.h"

GAMECLASS_C(CUrdBossState_Attack03_DashSting_L);
CLONE_C(CUrdBossState_Attack03_DashSting_L, CComponent)

void CUrdBossState_Attack03_DashSting_L::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CUrdBossState_Step_Idle>().lock()->Set_StepCloseCount(0);
	Get_Owner().lock()->Get_Component<CUrdBossState_Step_Idle>().lock()->Set_StepFarCount(0);
	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Attack03_DashSting_L::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 48:
		if (Is_Urd_Phase1())
		{
			TurnOn_Effect("Urd_WeaponShine");
		}
		return;
	case 106:
		if (Is_Urd_Phase1())
		{
			TurnOff_Effect("Urd_WeaponShine");
		}
		return;
	}
}

HRESULT CUrdBossState_Attack03_DashSting_L::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Attack03_DashSting_L::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CUrdBossState_Attack03_DashSting_L::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Attack03_DashSting_L|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd +=
		bind(&CUrdBossState_Attack03_DashSting_L::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Attack03_DashSting_L::Tick(_float fTimeDelta)
{
	CBossStateBase::Tick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
	}

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CUrdBossState_Attack03_DashSting_L::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
}

void CUrdBossState_Attack03_DashSting_L::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.4f);
	}

	Set_MoveScale(2.f);
	m_bAttackLookAtLimit = true;
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	if (m_pThisAnimationCom.lock())
	{
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
			bind(&CUrdBossState_Attack03_DashSting_L::Call_NextKeyFrame, this, placeholders::_1);
	}
}	


void CUrdBossState_Attack03_DashSting_L::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
	{
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
			bind(&CUrdBossState_Attack03_DashSting_L::Call_NextKeyFrame, this, placeholders::_1);
	}

	Set_MoveScale();
}

_bool CUrdBossState_Attack03_DashSting_L::Check_AndChangeNextState()
{
	if (!Check_Requirement())
	{
		return false;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		m_bAttackLookAtLimit = false;
	}

	return false;
}

void CUrdBossState_Attack03_DashSting_L::OnDestroy()
{
	__super::OnDestroy();
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Attack03_DashSting_L::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Attack03_DashSting_L::Free()
{
	__super::Free();
}
