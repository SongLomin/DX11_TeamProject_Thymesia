 #include "stdafx.h"
#include "BossBat/BatBossState_JumpSmash_SmarhL.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_JumpSmash_SmarhL);
CLONE_C(CBatBossState_JumpSmash_SmarhL, CComponent)

HRESULT CBatBossState_JumpSmash_SmarhL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_JumpSmash_SmarhL::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CBatBossState_JumpSmash_SmarhL::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_JumpSmashL");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_JumpSmash_SmarhL::Call_AnimationEnd, this);
}

void CBatBossState_JumpSmash_SmarhL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bRootStop)
	{
		_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");

		PxControllerFilters Filters = Filters;
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
	}
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_JumpSmash_SmarhL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}
	

	Check_AndChangeNextState();
}


void CBatBossState_JumpSmash_SmarhL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_bRootStop = true;

	m_bOne = true;

	m_bAttackLookAtLimit = true;

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	
}	

void CBatBossState_JumpSmash_SmarhL::OnStateEnd()
{
	__super::OnStateEnd();

}


void CBatBossState_JumpSmash_SmarhL::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CBatBossState_AttackIdle>().lock()->Set_AttackCount(1);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_JumpSmash_SmarhL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_JumpSmash_SmarhL::Call_AnimationEnd, this);
}

void CBatBossState_JumpSmash_SmarhL::Free()
{
}

_bool CBatBossState_JumpSmash_SmarhL::Check_AndChangeNextState()
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


	if (ComputeAngleWithPlayer() > 0.98f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
		m_bAttackLookAtLimit = false;
	}

	


	return false;
}

