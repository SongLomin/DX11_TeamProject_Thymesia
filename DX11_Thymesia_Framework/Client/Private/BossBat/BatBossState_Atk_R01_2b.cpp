#include "stdafx.h"
#include "BossBat/BatBossState_Atk_R01_2b.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "BoneNode.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "MobWeapon.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_Atk_R01_2b);
CLONE_C(CBatBossState_Atk_R01_2b, CComponent)

HRESULT CBatBossState_Atk_R01_2b::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Atk_R01_2b::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Atk_R01_2b::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_AttackR_01_2b");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Atk_R01_2b::Call_AnimationEnd, this, placeholders::_1);
}
	


void CBatBossState_Atk_R01_2b::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	if (m_bRootStop)
	{
		_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");

		PxControllerFilters Filters = Filters;
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
	}


	if (m_bTurnAttack)
	{
		_float fTurnValue = 0.14f / 0.383f;

		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnValue * 2.f);
	}
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Atk_R01_2b::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	

	Check_AndChangeNextState();
}

void CBatBossState_Atk_R01_2b::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

	if (In_iKeyIndex >= 70 && In_iKeyIndex <= 74)
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		return;
	}

	switch (In_iKeyIndex)
	{
	case 75:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.11f, 9.f, 3.f);
		break;
	}
	case 77:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.13f, 9.f, 3.f);
		break;
	}
	case 78:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.15f, 9.f, 3.f);
		break;
	}
	case 79:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.3f, 9.f, 3.f);
		break;
	}
	case 80:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.27f, 9.f, 3.f);
		break;
	}
	case 81:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.25f, 9.f, 3.f);
		break;
	}
	case 82:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.20, 9.f, 3.f);
		break;
	}
	case 83:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.19f, 9.f, 3.f);
		break;
	}
	case 84:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.17f, 9.f, 3.f);
		break;
	}
	case 85:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.16f, 9.f, 3.f);
		break;
	}
	case 86:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.14f, 9.f, 3.f);
		break;
	}
	case 87:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.12f, 9.f, 3.f);
		break;
	}
	case 88:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.11f, 9.f, 3.f);
		break;
	}
	case 89:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	case 90:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	case 206:
	{
		_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}

	case 302:
	{
		_matrix CombinedMatrix = m_pLeftHandBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_pModelCom.lock()->Get_TransformationMatrix())
			* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	}
}

void CBatBossState_Atk_R01_2b::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_bRootStop = true;

	m_bOne = true;

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.65f);
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_Atk_R01_2b::Call_NextAnimationKey, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	
}	


void CBatBossState_Atk_R01_2b::OnStateEnd()
{
	__super::OnStateEnd();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_Atk_R01_2b::Call_NextAnimationKey, this, placeholders::_1);

}



void CBatBossState_Atk_R01_2b::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CBatBossState_HellIdle>().lock()->Set_AttackCount(1);
	Get_Owner().lock()->Get_Component<CBatBossState_AttackIdle>().lock()->Set_AttackCount(1);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Atk_R01_2b::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Atk_R01_2b::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Atk_R01_2b::Free()
{

}

_bool CBatBossState_Atk_R01_2b::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	
	_float fPToMDistance = Get_DistanceWithPlayer();

	if (fPToMDistance <= 6.5f && m_bOne)
	{
		m_bRootStop = false;
		m_bOne = false;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 72 &&
		m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() < 95)
	{
		m_bTurnAttack = true;
	}


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 93)
	{
		m_bTurnAttack = false;
	}


	return false;
}

