#include "stdafx.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Weapon.h"
#include "GameManager.h"
#include "Player.h"
//#include "MonsterHPBar.h"
#include "Status.h"
//#include "ComboTimer.h"
#include "Attack_Area.h"
//#include "DamageUI.h"
#include "Status_Player.h"
#include "Status_Monster.h"
#include "Monster.h"
#include "Status.h"
#include "PhysXCharacterController.h"
#include "Status_Monster.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"
#include "EliteMonState/BigHandMan/BigHandManStateBase.h"

GAMECLASS_C(CBigHandManStateBase)

HRESULT CBigHandManStateBase::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_vShakingOffSet = { 0.f, 1.f, 0.f };
	return S_OK;
}

_bool CBigHandManStateBase::Check_RequirementAttackState()
{
	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
		return true;

	return false;
}

_bool CBigHandManStateBase::Check_RequirementDashState()
{
	if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
		return true;

	return false;
}

_bool CBigHandManStateBase::Check_RequirementRunState()
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

_uint CBigHandManStateBase::Check_RequirementDotState()
{

	DOT_RESULT DotResult = DOT_RESULT::DOT_RESULT_END;
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	_vector vMyDotPositon = m_pOwner.lock()->Get_Transform()->Get_Position();
	vMyDotPositon.m128_f32[1] = 0.f;
	_vector vOtherDotPositon = pCurrentPlayer.lock()->Get_Transform()->Get_Position();
	vOtherDotPositon.m128_f32[1] = 0.f;

	_vector vOtherColliderToPlayerClollider = XMVector3Normalize(vOtherDotPositon - vMyDotPositon);

	_vector vMyLookVecTor = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	vMyLookVecTor.m128_f32[1] = 0;
	vMyLookVecTor = XMVector3Normalize(vMyLookVecTor);

	_float fCos = XMVectorGetX(XMVector3Dot(vOtherColliderToPlayerClollider, vMyLookVecTor));

	if (fCos >= 0.173f && fCos <= 1)
		return (_uint)DOT_RESULT::LEFT;
	else if (fCos >= -0.173f && fCos <= 0.173)
		return (_uint)DOT_RESULT::MID;
	else
		return (_uint)DOT_RESULT::RIGHT;
}

_bool CBigHandManStateBase::Check_RequirementPlayerInRange(const _float& In_fRange)
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	_vector vPlayerPosition = pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVector3Length(vPlayerPosition - vMyPosition).m128_f32[0];
	return In_fRange >= fDistance;
}

void CBigHandManStateBase::Play_OnHitEffect()
{
	/*_vector vLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	vLook *= -1.f;
	_matrix ReverseLookMatrix = SMath::Bake_MatrixNormalizeUseLookVector(vLook);
	ReverseLookMatrix.r[3] = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	GET_SINGLE(CGameManager)->Use_EffectGroup("Hit_Monster1", ReverseLookMatrix);*/
	GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
	GET_SINGLE(CGameManager)->Use_EffectGroup("Hit_Monster2", m_pTransformCom);
}

void CBigHandManStateBase::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	__super::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::PLAYER_ATTACK)
	{
		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		weak_ptr<CAttackArea> pAttackArea = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner());
			
		weak_ptr<CStatus_Monster> pStatus = m_pOwner.lock()->Get_Component<CStatus_Monster>();
		
		weak_ptr<CCharacter> pOtherCharacter = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->Get_ParentObject();
		
		if (!pAttackArea.lock())
			return;

		_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];
		PxControllerFilters Filters;

		m_pTransformCom.lock()->LookAt(vSameHeightOtherColliderPosition);

		ATTACK_OPTION eAttackOption = pAttackArea.lock()->Get_OptionType();

		CStatus_Player::PLAYERDESC tPlayerDesc;
		_matrix vResultOtherWorldMatrix;

		pAttackArea.lock()->Get_ParentObject().lock()->Get_ComponentByType<CStatus>().lock()->Get_Desc(&tPlayerDesc);

		Play_OnHitEffect();

		_float fMagnifiedDamage = In_fDamage;
		_uint iRand = rand() % 8 + 1;

		switch (eAttackOption)
		{
		case Client::ATTACK_OPTION::NONE:
			fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, ATTACK_OPTION::NORMAL);
			break;
		case Client::ATTACK_OPTION::NORMAL:
			fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
			break;
		case Client::ATTACK_OPTION::PLAGUE:
			fMagnifiedDamage *= tPlayerDesc.m_fPlagueAtk;
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
			break;
		case Client::ATTACK_OPTION::SPECIAL_ATTACK:
			if (!m_pStatusCom.lock()->Is_Dead())
			{
				if (In_eHitType == HIT_TYPE::LEFT_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerCharacter().lock()->Change_State<CBigHandManState_HurtR>(0.05f);
				}

				else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerCharacter().lock()->Change_State<CBigHandManState_HurtL>(0.05f);
				}
				else if (In_eHitType == HIT_TYPE::DOWN_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerCharacter().lock()->Change_State<CBigHandManState_HurtL>(0.05f);
				}
				else if (In_eHitType == HIT_TYPE::NORMAL_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerCharacter().lock()->Change_State<CBigHandManState_HurtL>(0.05f);
				}
			}
			break;
		case Client::ATTACK_OPTION::STEALMONSTER:
			if (In_eHitType == HIT_TYPE::STEALMONSTER)
			{
				_matrix vOtherWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
				vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, 0.f, 1.f, 0.f));
				pOtherCharacter.lock()->Get_PhysX().lock()->Set_Position(
					vResultOtherWorldMatrix.r[3],
					GAMEINSTANCE->Get_DeltaTime(),
					Filters);
				pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_STEALCORVUS);
			}
			else if (In_eHitType == HIT_TYPE::LEFT_HIT)
			{
				Get_OwnerMonster()->Change_State<CBigHandManState_HurtR>();
			}
			else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
			{
				pOtherCharacter.lock()->OnStealMonsterSkill(Get_OwnerMonster()->Get_MonsterType());
				Get_OwnerMonster()->Change_State<CBigHandManState_HurtL>();
			}
			break;
		}
		_float3 vShakingOffset = pOtherCharacter.lock()->Get_CurState().lock()->Get_ShakingOffset();
		_vector vShakingOffsetToVector = XMLoadFloat3(&vShakingOffset);
		_float fShakingRatio = 0.01f * iRand;

		if (Get_OwnerCharacter().lock()->Get_CurState().lock() != Get_Owner().lock()->Get_Component<CBigHandManState_Stun_Start>().lock() &&
			Get_OwnerCharacter().lock()->Get_CurState().lock() != Get_Owner().lock()->Get_Component<CBigHandManState_Stun_Loop>().lock())
		{
			if (pStatus.lock()->Get_Desc().m_fCurrentHP_Green <= 0.f)
				Get_OwnerCharacter().lock()->Change_State<CBigHandManState_Stun_Start>(0.05f);
		}
		else
		{
			if (eAttackOption == ATTACK_OPTION::NONE ||
				eAttackOption == ATTACK_OPTION::NORMAL)
			pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_BIGHANDMANEXECUTION);

		}

		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffsetToVector, 0.1f + fShakingRatio, 1.f, 9.f, 0.5f);//�Ϲ� ����
		GAMEINSTANCE->Set_MotionBlur(0.05f);

		Get_OwnerMonster()->Set_RimLightDesc(4.5f, { 0.6f,0.f,0.f }, 0.9f);
	}
}

void CBigHandManStateBase::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CBigHandManStateBase::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CBigHandManStateBase::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CBigHandManStateBase::OnEventMessage(_uint iArg)
{
}

void CBigHandManStateBase::OnEventMessage(weak_ptr<CBase> pArg)
{
}

void CBigHandManStateBase::Free()
{
}
