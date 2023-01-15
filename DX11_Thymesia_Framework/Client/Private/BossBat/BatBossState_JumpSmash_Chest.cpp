#include "stdafx.h"
#include "BossBat/BatBossState_JumpSmash_Chest.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_JumpSmash_Chest);
CLONE_C(CBatBossState_JumpSmash_Chest, CComponent)

HRESULT CBatBossState_JumpSmash_Chest::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_JumpSmash_Chest::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_JumpSmash_Chest::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_JumpSmash_Chest");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");
	m_pChestBoneNode = m_pModelCom.lock()->Find_BoneNode("chest");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_JumpSmash_Chest::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_JumpSmash_Chest::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bRootStop)
	{
		_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");

		PxControllerFilters Filters;
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
	}
	
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_JumpSmash_Chest::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}



	Check_AndChangeNextState();
}

void CBatBossState_JumpSmash_Chest::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_iKeyIndex)
	{
	case 30:
	{
		_vector vPosition = m_pOwner.lock()->Get_Transform()->Get_Position();
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 0.9f, 0.3f);
		break;
	}
	case 87:
	{//chest
		_matrix CombinedMatrix = Get_ChestCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 0.9f, 0.3f);
		break;
	}
	case 92:
	{
		_matrix CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 0.9f, 0.3f);
		
		CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		vPosition = CombinedMatrix.r[3];
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 0.9f, 0.3f);

		GET_SINGLE(CGameManager)->Add_Shaking(XMVectorSet(0.f, -1.f, 0.f, 0.f), 0.2f, 1.f, 9.f, 0.5f);
		GAMEINSTANCE->Set_MotionBlur(0.05f);
		break;
	}
	case 107:
	{
		_matrix CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.13f, 0.9f, 0.3f);

		CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		vPosition = CombinedMatrix.r[3];
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.13f, 0.9f, 0.3f);

		break;
	}
	case 236:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 0.9f, 0.3f);

		break;
	}
	}
}

void CBatBossState_JumpSmash_Chest::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_bRootStop = true;

	m_bOne = true;

	m_pPhysXControllerCom.lock()->Enable_Gravity(false);

	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_ChestCheck(false);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_JumpSmash_Chest::Call_NextAnimationKey, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_JumpSmash_Chest::OnStateEnd()
{
	__super::OnStateEnd();

	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_JumpSmash_Chest::Call_NextAnimationKey, this, placeholders::_1);

}



void CBatBossState_JumpSmash_Chest::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_JumpSmash_Chest::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_JumpSmash_Chest::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_JumpSmash_Chest::Free()
{

}

_bool CBatBossState_JumpSmash_Chest::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer();

	if (fPToMDistance <= 6.5f && m_bOne)
	{
		m_bRootStop = false;
		m_bOne = false;
	}


	if (ComputeAngleWithPlayer() > 0.98f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
		m_bAttackLookAtLimit = false;
	}


	return false;
}

