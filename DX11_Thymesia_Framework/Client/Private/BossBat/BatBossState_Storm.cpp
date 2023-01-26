#include "stdafx.h"
#include "BossBat/BatBossState_Storm.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_Storm);
CLONE_C(CBatBossState_Storm, CComponent)

HRESULT CBatBossState_Storm::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Storm::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Storm::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_Storm_1");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");
	
}

void CBatBossState_Storm::Tick(_float fTimeDelta)
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


void CBatBossState_Storm::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}


void CBatBossState_Storm::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_pPhysXControllerCom.lock()->Enable_Gravity(false);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_Storm::OnStateEnd()
{
	__super::OnStateEnd();

	m_bSpecialAtk = false;
}







void CBatBossState_Storm::Free()
{

}

_bool CBatBossState_Storm::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (ComputeAngleWithPlayer() > 0.97f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
		m_bAttackLookAtLimit = false;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.99f)
	{
		if (m_bSpecialAtk)
		{
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_SonicBoom>(0.05f);
			return true;
		}
		else
		{
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_SonicBullet>(0.05f);
			return true;
		}		
		
	}


	return false;
}

