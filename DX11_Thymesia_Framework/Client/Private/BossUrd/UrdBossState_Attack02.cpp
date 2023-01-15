#include "stdafx.h"
#include "BossUrd/UrdBossState_Attack02.h"
#include "BossUrd/UrdBossState_Idle.h"
#include "BossUrd/UrdBossState_Step_Idle.h"
#include "Animation.h"

GAMECLASS_C(CUrdBossState_Attack02);
CLONE_C(CUrdBossState_Attack02, CComponent)

void CUrdBossState_Attack02::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_Attack(false);
	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_WalkStart(true);
	Get_Owner().lock()->Get_Component<CUrdBossState_Step_Idle>().lock()->Set_NoParryAttack(false);
	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Attack02::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 70:
		if (Is_Urd_Phase1())
		{
			TurnOn_Effect("Urd_WeaponShine");
		}
		return;
	case 128:
		if (Is_Urd_Phase1())
		{
			TurnOff_Effect("Urd_WeaponShine");
		}
		return;
	}
}

HRESULT CUrdBossState_Attack02::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Attack02::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_fTurnAtkSpeedRatio = 2.f;
	return S_OK;
}

void CUrdBossState_Attack02::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Attack02|BaseLayer");
	m_pModelCom.lock()->CallBack_AnimationEnd +=
		bind(&CUrdBossState_Attack02::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Attack02::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUrdBossState_Attack02::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
}

void CUrdBossState_Attack02::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;
	Set_MoveScale(2.f);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	if (m_pThisAnimationCom.lock())
	{
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
			bind(&CUrdBossState_Attack02::Call_NextKeyFrame, this, placeholders::_1);
	}
}

void CUrdBossState_Attack02::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
	{
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
			bind(&CUrdBossState_Attack02::Call_NextKeyFrame, this, placeholders::_1);
	}

	Set_MoveScale();
}

_bool CUrdBossState_Attack02::Check_AndChangeNextState()
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

void CUrdBossState_Attack02::OnDestroy()
{
	__super::OnDestroy();
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Attack02::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Attack02::Free()
{
	__super::Free();
}
