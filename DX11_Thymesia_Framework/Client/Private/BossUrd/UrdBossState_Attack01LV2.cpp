#include "stdafx.h"
#include "BossUrd/UrdBossState_Attack01LV2.h"
#include "BossUrd/Urd.h"
#include "BossUrd/UrdBossState_Idle.h"
#include "Model.h"
#include "Animation.h"
#include "BossUrd/UrdBossState_Attack02LV2C1.h"

GAMECLASS_C(CUrdBossState_Attack01LV2);
CLONE_C(CUrdBossState_Attack01LV2, CComponent)

void CUrdBossState_Attack01LV2::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
	{
		return;
	}

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Attack01LV2::Call_NextKeyFrame(const _uint& In_KeyIndex)
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

HRESULT CUrdBossState_Attack01LV2::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Attack01LV2::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CUrdBossState_Attack01LV2::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Attack01LV2|BaseLayer");
	m_pModelCom.lock()->CallBack_AnimationEnd +=
		bind(&CUrdBossState_Attack01LV2::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Attack01LV2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUrdBossState_Attack01LV2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
}

void CUrdBossState_Attack01LV2::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;
	Set_MoveScale(2.f);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 21);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	if (m_pThisAnimationCom.lock())
	{
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
			bind(&CUrdBossState_Attack01LV2::Call_NextKeyFrame, this, placeholders::_1);
	}
}	

void CUrdBossState_Attack01LV2::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
	{
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
			bind(&CUrdBossState_Attack01LV2::Call_NextKeyFrame, this, placeholders::_1);
	}

	Set_MoveScale();
}

_bool CUrdBossState_Attack01LV2::Check_AndChangeNextState()
{
	if (!Check_Requirement())
	{
		return false;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 30)
	{
		m_bAttackLookAtLimit = false;
	}

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 57)
	{
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack02LV2C1>(0.05f);
		return true;
	}

	return false;
}

void CUrdBossState_Attack01LV2::OnDestroy()
{
	__super::OnDestroy();
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Attack01LV2::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Attack01LV2::Free()
{
	__super::Free();
}
