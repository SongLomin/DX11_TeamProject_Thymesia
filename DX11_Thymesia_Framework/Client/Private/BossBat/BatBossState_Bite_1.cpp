#include "stdafx.h"
#include "BossBat/BatBossState_Bite_1.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "MobWeapon.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_Bite_1);
CLONE_C(CBatBossState_Bite_1, CComponent)

HRESULT CBatBossState_Bite_1::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Bite_1::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Bite_1::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_Bite_1");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");
	m_pChestBoneNode = m_pModelCom.lock()->Find_BoneNode("chest");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Bite_1::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Bite_1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bRootStop)
	{
		_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");

		PxControllerFilters Filters;
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
	}

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Bite_1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CBatBossState_Bite_1::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;
	if (In_iKeyIndex == 78)
	{
		_matrix CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.05f, 9.f, 3.f);
	}
	else if (In_iKeyIndex >= 171 && In_iKeyIndex <= 196 || In_iKeyIndex >= 288 && In_iKeyIndex <= 333)
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.03f, 9.f, 3.f);

		CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		vPosition = CombinedMatrix.r[3];
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.03f, 9.f, 3.f);

		CombinedMatrix = Get_ChestCombinedWorldMatrix();

		vPosition = CombinedMatrix.r[3];
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.03f, 9.f, 3.f);

		GET_SINGLE(CGameManager)->Add_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.05f, 1.f, 9.f, 0.4f);;

	}

}

void CBatBossState_Bite_1::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_bRootStop = true;

	m_bOne = true;

	//m_pPhysXControllerCom.lock()->Enable_Gravity(false);

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.3f);
	}

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_Bite_1::Call_NextAnimationKey, this, placeholders::_1);


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_Bite_1::OnStateEnd()
{
	__super::OnStateEnd();

	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_Bite_1::Call_NextAnimationKey, this, placeholders::_1);


}



void CBatBossState_Bite_1::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Bite_1::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Bite_1::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Bite_1::Free()
{

}

_bool CBatBossState_Bite_1::Check_AndChangeNextState()
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

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 242)
	{
		Get_OwnerCharacter().lock()->Change_State<CBatBossState_Bite_2>(0.05f);
		return true;
	}




	return false;
}

