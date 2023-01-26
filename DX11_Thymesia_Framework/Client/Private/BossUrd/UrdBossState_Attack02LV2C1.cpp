#include "stdafx.h"
#include "BossUrd/UrdBossState_Attack02LV2C1.h"
#include "BossUrd/UrdBossState_Idle.h"
#include "Animation.h"

GAMECLASS_C(CUrdBossState_Attack02LV2C1);
CLONE_C(CUrdBossState_Attack02LV2C1, CComponent)

void CUrdBossState_Attack02LV2C1::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Attack02LV2C1::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 34:
		if (Is_Urd_Phase1())
		{
			TurnOn_Effect("Urd_WeaponShine");
		}
		return;
	case 105:
		if (Is_Urd_Phase1())
		{
			TurnOff_Effect("Urd_WeaponShine");
		}
		return;
	}
}

HRESULT CUrdBossState_Attack02LV2C1::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Attack02LV2C1::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CUrdBossState_Attack02LV2C1::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Attack02LV2C1|BaseLayer");
	m_pModelCom.lock()->CallBack_AnimationEnd +=
		bind(&CUrdBossState_Attack02LV2C1::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Attack02LV2C1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUrdBossState_Attack02LV2C1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
}

void CUrdBossState_Attack02LV2C1::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Set_MoveScale(2.f);

	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_PhaseTwoSkillCount(1);

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 18);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	if (m_pThisAnimationCom.lock())
	{
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
			bind(&CUrdBossState_Attack02LV2C1::Call_NextKeyFrame, this, placeholders::_1);
	}
}	


void CUrdBossState_Attack02LV2C1::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
	{
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
			bind(&CUrdBossState_Attack02LV2C1::Call_NextKeyFrame, this, placeholders::_1);
	}

	Set_MoveScale();
}

_bool CUrdBossState_Attack02LV2C1::Check_AndChangeNextState()
{
	if (!Check_Requirement())
	{
		return false;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		m_bAttackLookAtLimit = false;
	}

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
	}

	return false;
}

void CUrdBossState_Attack02LV2C1::OnDestroy()
{
	__super::OnDestroy();
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Attack02LV2C1::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Attack02LV2C1::Free()
{
	__super::Free();
}
