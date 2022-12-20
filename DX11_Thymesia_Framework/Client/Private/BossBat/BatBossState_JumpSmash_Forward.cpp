#include "stdafx.h"
#include "BossBat/BatBossState_JumpSmash_ForwardL.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_JumpSmash_ForwardL);
CLONE_C(CBatBossState_JumpSmash_ForwardL, CComponent)

HRESULT CBatBossState_JumpSmash_ForwardL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_JumpSmash_ForwardL::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_JumpSmash_ForwardL::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|JUMPFORWARD");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_JumpSmash_ForwardL::Call_AnimationEnd, this);
}

void CBatBossState_JumpSmash_ForwardL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bRootStop)
	{
		_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));

		PxControllerFilters Filters = Filters;
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
	}


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_JumpSmash_ForwardL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}

	Check_AndChangeNextState();
}



void CBatBossState_JumpSmash_ForwardL::OnStateStart(const _float& In_fAnimationBlendTime)
{
  	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_bRootStop = true;

	m_bOne = true;

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_JumpSmash_ForwardL::OnStateEnd()
{
	__super::OnStateEnd();

	
}


void CBatBossState_JumpSmash_ForwardL::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_AttackCount(1);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_JumpSmash_ForwardL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_JumpSmash_ForwardL::Call_AnimationEnd, this);
}

void CBatBossState_JumpSmash_ForwardL::Free()
{

}

_bool CBatBossState_JumpSmash_ForwardL::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer();

	if (fPToMDistance <= 7.f && m_bOne)
	{
		m_bRootStop = false;
		m_bOne = false;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() == 50)
	{
		m_pPhysXControllerCom.lock()->Enable_Gravity(false);
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() == 188)
	{
		m_pPhysXControllerCom.lock()->Enable_Gravity(true);
	}


	if (ComputeAngleWithPlayer() > 0.98f)
	{
		Rotation_TargetToLookDir();
		m_bAttackLookAtLimit = false;
	}


	

	return false;
}

