#include "stdafx.h"
#include "BossBat/BatBossState_BackJump.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_BackJump);
CLONE_C(CBatBossState_BackJump, CComponent)

HRESULT CBatBossState_BackJump::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_BackJump::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_BackJump::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_Storm_1");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_BackJump::Call_AnimationEnd, this, placeholders::_1);

	
}

void CBatBossState_BackJump::Tick(_float fTimeDelta)
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
	
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_BackJump::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CBatBossState_BackJump::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;
	switch (In_iKeyIndex)
	{
	case 60:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 9.f, 3.f);
		break;
	}
	case 65:
	{
		_matrix CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 9.f, 3.f);
		break;
	}
	case 136:
	{//´Ù¸®
		_vector vPosition = m_pOwner.lock()->Get_Transform()->Get_Position();
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 9.f, 3.f);
		break;
	}
	case 197:
	{
		_vector vPosition = m_pOwner.lock()->Get_Transform()->Get_Position();
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 9.f, 3.f);
		break;
	}
	case 198:
	{
		_matrix CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 9.f, 3.f);
		break;
	}
	case 222:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 9.f, 3.f);
		break;
	}
	}
}


void CBatBossState_BackJump::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_HellIdle>().lock())
	{
		m_bSpecialAtk = true;
	}

	m_bAttackLookAtLimit = true;

	m_pPhysXControllerCom.lock()->Enable_Gravity(false);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_BackJump::Call_NextAnimationKey, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_BackJump::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	_float fPToMDistance = Get_DistanceWithPlayer();

	if (m_bSpecialAtk)
	{
		Get_OwnerCharacter().lock()->Change_State<CBatBossState_Sp>(0.05f);
	}
	else
	{
		if (fPToMDistance <= 15)
		{
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_Charge>(0.05f);
			return;
		}
		else
		{
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_SonicBullet>(0.05f);
			return;
		}
		
	}


}


void CBatBossState_BackJump::OnStateEnd()
{
	__super::OnStateEnd();


	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_BackJump::Call_NextAnimationKey, this, placeholders::_1);


	m_bSpecialAtk = false;
}


void CBatBossState_BackJump::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_BackJump::Call_AnimationEnd, this, placeholders::_1);
}





void CBatBossState_BackJump::Free()
{

}

_bool CBatBossState_BackJump::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (ComputeAngleWithPlayer() > 0.97f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
		m_bAttackLookAtLimit = false;
	}

	
	return false;
}

