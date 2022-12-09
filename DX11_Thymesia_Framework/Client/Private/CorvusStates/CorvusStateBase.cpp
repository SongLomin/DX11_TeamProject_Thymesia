#include "stdafx.h"
#include "CorvusStates/CorvusStateBase.h"
#include "GameInstance.h"
#include "Collider.h"
#include "CorvusStates/CorvusStates.h"
//#include "MonsterWeapon.h"
#include "Status.h"
#include "Attack_Area.h"
#include "Status.h"
#include "Status_Player.h"
#include "Character.h"
#include "Status_Monster.h"
#include "Monster.h"
#include "NorMonStates.h"
#include "PhysXCharacterController.h"
#include "Interaction_Ladder.h"
#include "VargStates.h"
#include "BoneNode.h"
#include "Model.h"
#include "RequirementChecker.h"


GAMECLASS_C(CCorvusStateBase)

_bool CCorvusStateBase::Check_RequirementAttackState()
{
	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return true;
	}

	return false;
}

_bool CCorvusStateBase::Check_RequirementDashState()
{
	if (!KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		return false;
	}

	//if (!m_pStatusCom.lock()->Requirment_Dash())
	//{
	//	return false;
	//}

	return true;
}

_bool CCorvusStateBase::Check_RequirementJoggingState()
{
	if (KEY_INPUT(KEY::ALT, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::W, KEY_STATE::HOLD)
			|| KEY_INPUT(KEY::A, KEY_STATE::HOLD)
			|| KEY_INPUT(KEY::S, KEY_STATE::HOLD)
			|| KEY_INPUT(KEY::D, KEY_STATE::HOLD))
		{
			return true;
		}
	}


	return false;
}

_bool CCorvusStateBase::Check_RequirementSprintState()
{
	if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::W, KEY_STATE::HOLD)
			|| KEY_INPUT(KEY::A, KEY_STATE::HOLD)
			|| KEY_INPUT(KEY::S, KEY_STATE::HOLD)
			|| KEY_INPUT(KEY::D, KEY_STATE::HOLD))
		{
			return true;
		}
	}


	return false;
}

_bool CCorvusStateBase::Check_RequirementRunState()
{
	if (KEY_INPUT(KEY::W, KEY_STATE::HOLD)
		|| KEY_INPUT(KEY::A, KEY_STATE::HOLD)
		|| KEY_INPUT(KEY::S, KEY_STATE::HOLD)
		|| KEY_INPUT(KEY::D, KEY_STATE::HOLD))
	{
		return true;
	}

	return false;
}

_bool CCorvusStateBase::Check_RequirementAVoidState()
{

	if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		return true;
	}
	return false;
}

_bool CCorvusStateBase::Check_RequirementParryState()
{
	if (KEY_INPUT(KEY::F, KEY_STATE::TAP))
	{
		return true;
	}
	return false;
}

_bool CCorvusStateBase::Check_RequirementUltimateState()
{

	if (KEY_INPUT(KEY::V, KEY_STATE::HOLD))
	{
		return true;
	}

	return false;
}

_bool CCorvusStateBase::Check_RequirementHealingState()
{
	if (KEY_INPUT(KEY::Q, KEY_STATE::TAP))
	{
		return true;
	}

	return false;
}

_bool CCorvusStateBase::Check_RequirementClawAttackState()
{
	if (KEY_INPUT(KEY::RBUTTON, KEY_STATE::AWAY))
	{
		return true;
	}

	return false;
}

_bool CCorvusStateBase::Check_RequirementClawAttackHoldState()
{
	if (KEY_INPUT(KEY::RBUTTON, KEY_STATE::HOLD))
	{
		return true;
	}

	return false;
}

_bool CCorvusStateBase::Check_RequirementObjectInteraction()
{
	if (KEY_INPUT(KEY::E, KEY_STATE::TAP))
	{
		return true;
	}

	return false;
}

_bool CCorvusStateBase::Check_RequirementUpLadderState()
{
	if (KEY_INPUT(KEY::W, KEY_STATE::HOLD))
	{
		return true;
	}

	return false;
}

_bool CCorvusStateBase::Check_RequirementDownLadderState()
{
	if (KEY_INPUT(KEY::S, KEY_STATE::HOLD))
	{
		return true;
	}

	return false;
}

_bool CCorvusStateBase::Check_RequirementExcuteState(weak_ptr<CGameObject>& Out_pGameObject)
{
	list<weak_ptr<CGameObject>>  pGameObjects = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::GROOGYMOSNTER);


	//몬스터랑 거리비교를해야함

	return Get_NearGameObjectInDistance(Out_pGameObject, pGameObjects, 5.f);
}

void CCorvusStateBase::Check_AndChangeHitState(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	weak_ptr<CStatus_Player> pStatus = Weak_StaticCast<CStatus_Player>(m_pStatusCom);

	if (pStatus.lock()->Is_Dead())
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_Die>();
	}
	else if (In_eHitType == HIT_TYPE::NORMAL_HIT)
	{
		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

		m_pTransformCom.lock()->LookAt(vSameHeightOtherColliderPosition);


		_vector vCorssPotision = pMyCollider.lock()->Get_CurrentPosition();
		_vector vOtherDoCorssPositon = pOtherCollider.lock()->Get_CurrentPosition();
		_vector vPlayerColToMonsterColDireciton = XMVector3Normalize(vOtherDoCorssPositon - vCorssPotision);

		_vector vMyColiderLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
		vMyColiderLook = XMVector3Normalize(vMyColiderLook);

		_vector fCross = XMVector3Cross(vMyColiderLook, vPlayerColToMonsterColDireciton);
		_float  fYCross = XMVectorGetY(XMVector3Cross(vMyColiderLook, fCross));
		//_float fDir = XMVector3Dot(fCross, XMVectorSet(0.f, 1.f, 0.f, 0.f)).m128_f32[0];

		if (fYCross > 0.f) //양수 오른쪽
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_HurtR>();
		}
		else // 음수
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_HurtL>();
		}



		//왼쪽오른쪽구분밥먹고합시다 ㅇㅈ?어 ㅇㅈ


		//Get_OwnerPlayer()->Change_State<CCorvusState_HurtL>();
	}
	else if (In_eHitType == HIT_TYPE::DOWN_HIT)
	{
		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

		m_pTransformCom.lock()->LookAt(vSameHeightOtherColliderPosition);

		Get_OwnerPlayer()->Change_State<CCorvusState_HurtXXL>();
	}


}

_int CCorvusStateBase::Check_AndChangeSuccessParrying(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{

	CORSS_RESULT CorssResult = CORSS_RESULT::CORSS_END;

	//외적
	_vector vCorssPotision = pMyCollider.lock()->Get_CurrentPosition();
	_vector vOtherDoCorssPositon = pOtherCollider.lock()->Get_CurrentPosition();
	_vector vPlayerColToMonsterColDireciton = XMVector3Normalize(vOtherDoCorssPositon - vCorssPotision);

	_vector vMyColiderLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	vMyColiderLook = XMVector3Normalize(vMyColiderLook);

	_vector fCross = XMVector3Cross(vMyColiderLook, vPlayerColToMonsterColDireciton);
	_float  fYCross = XMVectorGetY(XMVector3Cross(vMyColiderLook, fCross));
	//_float fDir = XMVector3Dot(fCross, XMVectorSet(0.f, 1.f, 0.f, 0.f)).m128_f32[0];

	if (fYCross > 0.f) //양수 오른쪽
	{
		CorssResult = CORSS_RESULT::RIGHT;
	}
	else // 음수
	{
		CorssResult = CORSS_RESULT::LEFT;
	}

	//내적
	_vector vMyDotPositon = pMyCollider.lock()->Get_CurrentPosition();
	vMyDotPositon.m128_f32[1] = 0.f;
	_vector vOtherDotPositon = pOtherCollider.lock()->Get_CurrentPosition();
	vOtherDotPositon.m128_f32[1] = 0.f;

	_vector vOtherColliderToPlayerClollider = XMVector3Normalize(vOtherDotPositon - vMyDotPositon);

	_vector vMyLookVecTor = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	vMyLookVecTor.m128_f32[1] = 0;
	vMyLookVecTor = XMVector3Normalize(vMyLookVecTor);

	_float fCos = XMVectorGetX(XMVector3Dot(vOtherColliderToPlayerClollider, vMyLookVecTor));


	if (CorssResult == CORSS_RESULT::LEFT)
	{
		if (fCos >= 0.f && fCos <= 0.7f) // 0도45도 또는 135도 180도
		{
			return (_uint)PARRY_SUCCESS::LEFTUP;
		}
		else
		{
			return (_uint)PARRY_SUCCESS::LEFT;
		}
	
		


	}
	else if (CorssResult == CORSS_RESULT::RIGHT)
	{
		if (fCos >= 0.f && fCos <= 0.7f) //0도~45도 이하
		{
			return (_uint)PARRY_SUCCESS::RIGHTUP;
		}
		else
		{
			return (_uint)PARRY_SUCCESS::RIGHT;
		}


	}

	return (_uint)PARRY_SUCCESS::FAIL;
}

_bool CCorvusStateBase::Check_AndChangeLadderState(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	if (!KEY_INPUT(KEY::E, KEY_STATE::TAP))
	{
		return false;
	}


	_bool bChanged = true;
	_matrix vOtherWorldMatrix = pOtherCollider.lock()->Get_Owner().lock()->Get_Transform()->Get_WorldMatrix();
	_vector vMyPos = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	_matrix vResultOtherWorldMatrix;
	PxControllerFilters Filters;
	const _flag CollisionObjectFlags = Get_OwnerPlayer()->Get_CollisionObjectFlags();

	switch ((COLLISION_LAYER)pOtherCollider.lock()->Get_CollisionLayer())
	{
		
	case Client::COLLISION_LAYER::LADDER_UP:
	{
		if (!Get_OwnerPlayer()->Get_LadderCheck())
		{
			_float fHeightOffset = Weak_Cast<CInteraction_Ladder>(pOtherCollider.lock()->Get_Owner()).lock()->Get_UpLadderHeight();

			vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, fHeightOffset, 0.4f, 0.f));

			m_pPhysXControllerCom.lock()->Set_Position(
				vResultOtherWorldMatrix.r[3],
				GAMEINSTANCE->Get_DeltaTime(),
				Filters);
			//m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vOtherPos + vLadderOffSetDown + XMVectorSet(0.f, 0.f, 0.f, 0.f));
			//m_pTransformCom.lock()->LookAt2D(vOtherWorldMatrix.r[3]);
			m_pTransformCom.lock()->Set_Look2D(vOtherWorldMatrix.r[2]);
			m_pPhysXControllerCom.lock()->Enable_Gravity(false);
			Get_OwnerPlayer()->Set_LadderCheck(true);		
			Get_OwnerPlayer()->Change_State<CCorvusState_Climb_R_Up_Down_End>();
			
		}
	
	}		
		break;

	case Client::COLLISION_LAYER::LADDER_DOWN:
		if (!Get_OwnerPlayer()->Get_LadderCheck())
		{
			vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, 0.02f, -1.f, 0.f));
			m_pPhysXControllerCom.lock()->Enable_Gravity(false);
			m_pPhysXControllerCom.lock()->Set_Position(
				vResultOtherWorldMatrix.r[3],
				GAMEINSTANCE->Get_DeltaTime(),
				Filters);
			m_pTransformCom.lock()->Set_Look2D(vOtherWorldMatrix.r[2]);
			Get_OwnerPlayer()->Set_LadderCheck(true);
			Get_OwnerPlayer()->Change_State<CCorvusState_Climb_Start>();
		}
		break;

	case Client::COLLISION_LAYER::ELEVATOR:
		break;

	case Client::COLLISION_LAYER::DOOR:
		break;

	case Client::COLLISION_LAYER::CHECKPOINT:
		vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, 0.02f, -1.f, 0.f));
		m_pPhysXControllerCom.lock()->Enable_Gravity(false);
		m_pPhysXControllerCom.lock()->Set_Position(
			vResultOtherWorldMatrix.r[3],
			GAMEINSTANCE->Get_DeltaTime(),
			Filters);
		m_pTransformCom.lock()->Set_Look2D(vOtherWorldMatrix.r[2]);
		Get_OwnerPlayer()->Set_LadderCheck(true);
		Get_OwnerPlayer()->Change_State<CCorvusState_CheckPointStart>();
		break;

	default:
		bChanged = false;
		break;
	}

	return bChanged;
}

_bool CCorvusStateBase::Check_RequirementAttackClose(weak_ptr<CGameObject>& Out_pGameObject)
{
	list<weak_ptr<CGameObject>>  pGameObjects = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::MONSTER);


	if (!Out_pGameObject.lock())
		return false;



	return false();
}

void CCorvusStateBase::Call_OtherControllerHit(const PxControllersHit& In_hit)
{
	shared_ptr<CRequirement_State> pReq_Once = make_shared<CRequirement_State>();
	pReq_Once->Init_Req(m_pOwnerFromPlayer, m_iStateIndex);

	m_pOwnerFromPlayer.lock()->Get_Requirement("RootMotion").lock()->Add_Requirement(pReq_Once);
}

void CCorvusStateBase::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	__super::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);



	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::MONSTER_ATTACK)
	{
		
	

		weak_ptr<CStatus_Player> pStatus = Weak_StaticCast<CStatus_Player>(m_pStatusCom);
		PxControllerFilters Filters;

		if (!pStatus.lock())
		{
			MSG_BOX("Error: Can't Find CStatus_Player From CorvusStateBase!");
		}

		weak_ptr<CAttackArea>	pAttackArea = Weak_StaticCast<CAttackArea>(pOtherCollider.lock()->Get_Owner());
		weak_ptr<CCharacter>	pMonsterFromCharacter = pAttackArea.lock()->Get_ParentObject();
		weak_ptr<CActor>        pActorMonster = Weak_StaticCast<CActor>(pMonsterFromCharacter);
		weak_ptr<CStatus_Monster>	pMonsterStatusCom = pMonsterFromCharacter.lock()->Get_Component<CStatus_Monster>();
	
		_matrix vResultOtherWorldMatrix;

		if (!pMonsterStatusCom.lock())
			MSG_BOX("Error : Can't Find CStatus_Monster From CorvusStateBase");

		//pMonsterStatusCom.lock()->OnEventMessage();
		ATTACK_OPTION eAttackOption = pAttackArea.lock()->Get_OptionType();

		switch (eAttackOption)
		{

		case Client::ATTACK_OPTION::SPECIAL_ATTACK:
			_matrix vOtherWorldMatrix = pMonsterFromCharacter.lock()->Get_Transform()->Get_WorldMatrix();
			pMonsterFromCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_CATCH);
			vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, 0.f, -2.f, 0.f));
			m_pPhysXControllerCom.lock()->Set_Position(
				vResultOtherWorldMatrix.r[3],
				GAMEINSTANCE->Get_DeltaTime(),
				Filters);
			m_pTransformCom.lock()->Set_Look2D(vOtherWorldMatrix.r[2]);
			Get_OwnerPlayer()->Change_State<CCorvusState_RaidAttack1Hurt>();
			break;
		default:
 			Check_AndChangeHitState(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
			pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);
			break;
		}

		
		
		
		
	


	
	}




}

void CCorvusStateBase::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);

}

void CCorvusStateBase::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);

	Check_AndChangeLadderState(pMyCollider, pOtherCollider);
}

void CCorvusStateBase::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);

}

void CCorvusStateBase::OnEventMessage(_uint iArg)
{
	/*if ((_uint)EVENT_TYPE::ON_EXCUTION_NORMOB)
	{
		Get_OwnerCharacter().lock()->Change_State<CCorvusState_NorMob_Execution>();
	}*/
}

void CCorvusStateBase::Free()
{
}
