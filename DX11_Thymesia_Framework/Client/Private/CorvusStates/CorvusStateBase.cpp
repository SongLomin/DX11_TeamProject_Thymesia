#include "stdafx.h"
#include "CorvusStates/CorvusStateBase.h"
#include "Requirement_State.h"
#include "RequirementChecker.h"
#include "UIManager.h"
#include "Status_Player.h"
#include "GameManager.h"
#include "CorvusStates/CorvusState_Die.h"
#include "Attack_Area.h"
#include "Collider.h"
#include "CorvusStates/CorvusState_HurtR.h"
#include "CorvusStates/CorvusState_HurtL.h"
#include "CorvusStates/CorvusState_HurtXXL.h"
#include "CorvusStates/CorvusState_HurtXL.h"
#include "CorvusStates/CorvusState_HurtBlown.h"
#include "Interaction_Ladder.h"
#include "SMath.h"
#include "PhysXController.h"
#include "CorvusStates/CorvusState_Climb_L_Up_Down_End.h"
#include "CorvusStates/CorvusState_Climb_Start.h"
#include "CorvusStates/CorvusState_CheckPointStart.h"
#include "Status_Monster.h"
#include "CorvusStates/StateExecution/CorvusState_RaidAttack1Hurt.h"
#include "CorvusStates/CorvusState_Headache_Start.h"
#include "CorvusStates/CorvusState_KnockBack.h"
#include "Monster.h"

GAMECLASS_C(CCorvusStateBase)

void CCorvusStateBase::Call_OtherControllerHit(const PxControllersHit& In_hit)
{
	__super::Call_OtherControllerHit(In_hit);

	shared_ptr<CRequirement_State> pReq_Once = make_shared<CRequirement_State>();
	pReq_Once->Init_Req(m_pOwnerFromPlayer, m_iStateIndex);

	m_pOwnerFromPlayer.lock()->Get_Requirement("RootMotion").lock()->Add_Requirement(pReq_Once);
}

void CCorvusStateBase::OnEventMessage(_uint iArg)
{
	/*if ((_uint)EVENT_TYPE::ON_EXCUTION_NORMOB)
	{
		Get_OwnerCharacter().lock()->Change_State<CCorvusState_NorMob_Execution_matrix CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.25f, 9.f, 3.f);

		CombinedMatrix = Get_RightFootCombinedWorldMatrix();

		vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.25f, 9.f, 3.f);>();
	}*/
}

void CCorvusStateBase::Free()
{
}

HRESULT CCorvusStateBase::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if(Get_Owner().lock()->Get_CreatedLevel() == LEVEL_STAGE2)
		CallBack_WaterWave+= bind(&CCorvusStateBase::Add_WaveDesc, this, placeholders::_1);
	

	return S_OK;
}

void CCorvusStateBase::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CallBack_WaterWave(fTimeDelta);
}

_bool CCorvusStateBase::Check_RequirementAttackState()
{
	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return true;
	}

	return false;
}

_bool CCorvusStateBase::Check_RequirementFeatherAttackState()
{
	if (KEY_INPUT(KEY::V, KEY_STATE::TAP))
		return true;

	return false;
}

_bool CCorvusStateBase::Check_RequirementDashState()
{
	if (!KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		return false;
	}

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
		return true;

	return false;
}

_bool CCorvusStateBase::Check_RequirementHealingState()
{
	if (KEY_INPUT(KEY::Q, KEY_STATE::TAP))
	{
		if (GET_SINGLE(CUIManager)->Is_OpenedMenu())
		{
			return false;
		}

		weak_ptr<CStatus_Player> pStatus = GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status();
		if (pStatus.lock()->Get_UseableCurrentPotion())
		{
			return true;
		}
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

		m_pTransformCom.lock()->LookAt2D(vSameHeightOtherColliderPosition);

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

		m_pTransformCom.lock()->LookAt2D(vSameHeightOtherColliderPosition);

		Get_OwnerPlayer()->Change_State<CCorvusState_HurtXXL>();
	}

	else if (In_eHitType == HIT_TYPE::UPPER_HIT)
	{
		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

		m_pTransformCom.lock()->LookAt2D(vSameHeightOtherColliderPosition);

		Get_OwnerPlayer()->Change_State<CCorvusState_HurtXL>();
	}
	else if (In_eHitType == HIT_TYPE::WARNING)
	{
		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

		m_pTransformCom.lock()->LookAt2D(vSameHeightOtherColliderPosition);

		Get_OwnerPlayer()->Change_State<CCorvusState_HurtBlown>();
	}
	else if (In_eHitType == HIT_TYPE::NOPARRYATTACK)
	{
		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

		m_pTransformCom.lock()->LookAt2D(vSameHeightOtherColliderPosition);

		Get_OwnerPlayer()->Change_State<CCorvusState_HurtXL>();

	}


	m_pOwnerFromPlayer.lock()->Set_RimLightDesc(4.5f, {0.6f,0.f,0.f}, 0.9f);
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

			vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, fHeightOffset -0.12f, 0.485f, 0.f));
			m_pPhysXControllerCom.lock()->Set_Position(
				vResultOtherWorldMatrix.r[3],
				GAMEINSTANCE->Get_DeltaTime(),
				Filters);
			//m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vOtherPos + vLadderOffSetDown + XMVectorSet(0.f, 0.f, 0.f, 0.f));
			//m_pTransformCom.lock()->LookAt2D(vOtherWorldMatrix.r[3]);
			m_pTransformCom.lock()->Set_Look2D(vOtherWorldMatrix.r[2]);
			m_pPhysXControllerCom.lock()->Enable_Gravity(false);
			Get_OwnerPlayer()->Set_LadderCheck(true);		
			Get_OwnerPlayer()->Change_State<CCorvusState_Climb_L_Up_Down_End>();
			
			weak_ptr<CInteraction_Ladder> pLadder = Weak_Cast<CInteraction_Ladder>(pOtherCollider.lock()->Get_Owner());

			if (pLadder.lock())
				pLadder.lock()->Set_RenderOutLine(false);
		}
	
	}		
		break;

	case Client::COLLISION_LAYER::LADDER_DOWN:
		if (!Get_OwnerPlayer()->Get_LadderCheck())
		{
			vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, -0.02f, -1.02f, 0.f));
			m_pPhysXControllerCom.lock()->Enable_Gravity(false);
			m_pPhysXControllerCom.lock()->Set_Position(
				vResultOtherWorldMatrix.r[3],
				GAMEINSTANCE->Get_DeltaTime(),
				Filters);
			m_pTransformCom.lock()->Set_Look2D(vOtherWorldMatrix.r[2]);
			Get_OwnerPlayer()->Set_LadderCheck(true);
			Get_OwnerPlayer()->Change_State<CCorvusState_Climb_Start>();

			weak_ptr<CInteraction_Ladder> pLadder = Weak_Cast<CInteraction_Ladder>(pOtherCollider.lock()->Get_Owner());

			if (pLadder.lock())
				pLadder.lock()->Set_RenderOutLine(false);
		}
		break;

	case Client::COLLISION_LAYER::ELEVATOR:
		break;

	case Client::COLLISION_LAYER::DOOR:
		break;

	case Client::COLLISION_LAYER::CHECKPOINT:
		vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(-0.2f, 0.f, -0.5f, 0.f));
		m_pPhysXControllerCom.lock()->Set_Position(
			vResultOtherWorldMatrix.r[3],
			GAMEINSTANCE->Get_DeltaTime(),
			Filters);
		m_pTransformCom.lock()->Set_Look2D(-vOtherWorldMatrix.r[2]);
		Get_OwnerPlayer()->Set_LadderCheck(true);
		Get_OwnerPlayer()->Change_State<CCorvusState_CheckPointStart>();
		break;
	case Client::COLLISION_LAYER::ITEM:
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

	return false;
}

void CCorvusStateBase::Add_WaveDesc(_float In_fTimeDelta)
{
	m_fTimeAcc += In_fTimeDelta;

	if (m_fTimeAcc > 0.05f)
	{
		_vector vPosition = m_pOwner.lock()->Get_Transform()->Get_Position();

		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.05f, 0.9f, 3.f);
		m_fTimeAcc = 0.f;
	}
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
		weak_ptr<CStatus_Monster>	pMonsterStatusCom = Weak_StaticCast<CStatus_Monster>(pMonsterFromCharacter.lock()->Get_Status());
		_matrix vOtherWorldMatrix = pMonsterFromCharacter.lock()->Get_Transform()->Get_WorldMatrix();
		_matrix vResultOtherWorldMatrix;

		if (!pMonsterStatusCom.lock())
			MSG_BOX("Error : Can't Find CStatus_Monster From CorvusStateBase");

		CMonster::STATE_LINK_MONSTER_DESC tMonsterDesc;
		tMonsterDesc.Reset();

		//pMonsterStatusCom.lock()->OnEventMessage();
		ATTACK_OPTION eAttackOption = pAttackArea.lock()->Get_OptionType();



		switch (eAttackOption)
		{
		case Client::ATTACK_OPTION::SPECIAL_ATTACK:
			pMonsterFromCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_CATCH);
			vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, 0.f, -1.75f, 0.f));
			m_pPhysXControllerCom.lock()->Set_Position(
				vResultOtherWorldMatrix.r[3],
				GAMEINSTANCE->Get_DeltaTime(),
				Filters);
			m_pTransformCom.lock()->Set_Look2D(vOtherWorldMatrix.r[2]);
			Get_OwnerPlayer()->Change_State<CCorvusState_RaidAttack1Hurt>();
			break;
		case Client::ATTACK_OPTION::PLAGUE:
			Get_OwnerPlayer()->Change_State<CCorvusState_Headache_Start>();
			break;
		case Client::ATTACK_OPTION::KNOCKBACK:
			m_pTransformCom.lock()->Set_Look2D(-vOtherWorldMatrix.r[2]);
			pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);	
			Get_OwnerPlayer()->Change_State<CCorvusState_KnockBack>();		
			break;
		case Client::ATTACK_OPTION::MULTI_HIT:
			pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);
			break;
		default:
			pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);
			Check_AndChangeHitState(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
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

void CCorvusStateBase::HitEffectSound()
{
	_uint iRandom = rand() % 3;

	switch (iRandom)
	{
		case 0: GAMEINSTANCE->PlaySound3D("Impact_Blade_Metal_Medium_Armor_Flesh_03.wav", 1.f, m_pTransformCom.lock()->Get_Position()); break;
		case 1: GAMEINSTANCE->PlaySound3D("Impact_Blade_Metal_Medium_Armor_Flesh_04.wav", 1.f, m_pTransformCom.lock()->Get_Position()); break;
		case 2: GAMEINSTANCE->PlaySound3D("Impact_Blade_Metal_Medium_Armor_Flesh_05.wav", 1.f, m_pTransformCom.lock()->Get_Position()); break;
	}
}
