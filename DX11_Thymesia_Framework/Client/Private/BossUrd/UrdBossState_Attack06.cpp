#include "stdafx.h"
#include "BossUrd/UrdBossState_Attack06.h"
#include "BossUrd/UrdBossState_Idle.h"
#include "Animation.h"

GAMECLASS_C(CUrdBossState_Attack06);
CLONE_C(CUrdBossState_Attack06, CComponent)

void CUrdBossState_Attack06::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Attack06::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 100:
		if (Is_Urd_Phase1())
		{
			TurnOff_Effect("Urd_WeaponShine");
		}
		return;
	}
}

HRESULT CUrdBossState_Attack06::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Attack06::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CUrdBossState_Attack06::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Attack06|BaseLayer");
	m_pModelCom.lock()->CallBack_AnimationEnd +=
		bind(&CUrdBossState_Attack06::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Attack06::Tick(_float fTimeDelta)
{
	CBossStateBase::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CUrdBossState_Attack06::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
}

void CUrdBossState_Attack06::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Set_MoveScale(2.f);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	if (m_pThisAnimationCom.lock())
	{
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
			bind(&CUrdBossState_Attack06::Call_NextKeyFrame, this, placeholders::_1);
	}
}

void CUrdBossState_Attack06::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
	{
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
			bind(&CUrdBossState_Attack06::Call_NextKeyFrame, this, placeholders::_1);
	}

	Set_MoveScale();
}

_bool CUrdBossState_Attack06::Check_AndChangeNextState()
{
	if (!Check_Requirement())
	{
		return false;
	}

	return false;
}

void CUrdBossState_Attack06::OnDestroy()
{
	__super::OnDestroy();
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Attack06::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Attack06::Free()
{
	__super::Free();
}
