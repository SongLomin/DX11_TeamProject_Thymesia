#include "stdafx.h"
#include "BossUrd/UrdBossState_AttackComboB2.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_AttackComboB2);
CLONE_C(CUrdBossState_AttackComboB2, CComponent)

HRESULT CUrdBossState_AttackComboB2::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_AttackComboB2::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_AttackComboB2::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Attack01LV2|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_AttackComboB2::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_AttackComboB2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta * 2.f);
	}
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_AttackComboB2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_AttackComboB2::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex,26);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_AttackComboB2::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_AttackComboB2::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_AttackComboB2::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_AttackComboB2::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_AttackComboB2::Free()
{

}

_bool CUrdBossState_AttackComboB2::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 36)
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 41)
	{
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_AttackComboB3>(0.05f);
		return true;
	}

	return false;
}

