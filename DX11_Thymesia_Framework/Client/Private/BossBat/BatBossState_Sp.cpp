#include "stdafx.h"
#include "BossBat/BatBossState_Sp.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_Sp);
CLONE_C(CBatBossState_Sp, CComponent)

HRESULT CBatBossState_Sp::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Sp::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Sp::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_SP01");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Sp::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Sp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
	
	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}

	if (m_bShaking)
	{
		GET_SINGLE(CGameManager)->Add_Shaking(XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.15f, 1.f, 9.f, 0.9f);
		GAMEINSTANCE->Set_MotionBlur(0.05f);
	}
	else if (m_bScreamShaking)
	{
		GET_SINGLE(CGameManager)->Add_Shaking(XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.2f, 1.f, 7.f, 0.9f);
		GAMEINSTANCE->Set_MotionBlur(0.05f);

	}
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Sp::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CBatBossState_Sp::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;
	switch (In_iKeyIndex)
	{
	case 36:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	case 43:
		m_bScreamShaking = true;
		break;
	case 137:
		m_bScreamShaking = false;
		break;
	case 272:
		m_bShaking = true;
		break;
	case 349:
		m_bShaking = false;
		break;

	case 365:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	case 370:
	{
		_matrix CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	case 461:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	case 489:
	{
		_matrix CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	}
}

void CBatBossState_Sp::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_Sp::Call_NextAnimationKey, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_Sp::OnStateEnd()
{
	__super::OnStateEnd();

	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_Sp::Call_NextAnimationKey, this, placeholders::_1);

}



void CBatBossState_Sp::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_HeelScream(false);
	Get_Owner().lock()->Get_Component<CBatBossState_Storm>().lock()->Set_SpecialAtk(true);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Storm>(0.05f);
}

void CBatBossState_Sp::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Sp::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Sp::Free()
{

}

_bool CBatBossState_Sp::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	if (ComputeAngleWithPlayer() > 0.97f)
	{
		Rotation_TargetToLookDir();
		m_bAttackLookAtLimit = false;
	}

	return false;
}

