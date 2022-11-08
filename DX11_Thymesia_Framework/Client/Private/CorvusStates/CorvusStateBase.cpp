#include "stdafx.h"
#include "CorvusStates/CorvusStateBase.h"
#include "GameInstance.h"
#include "Collider.h"
#include "CorvusStates/CorvusStates.h"
//#include "MonsterWeapon.h"
#include "Status.h"
#include "Attack_Area.h"

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

	if (!m_pStatusCom.lock()->Requirment_Dash())
	{
		return false;
	}

	return true;
}

_bool CCorvusStateBase::Check_RequirementRunState()
{
	if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
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

_bool CCorvusStateBase::Check_RequirementUltimateState()
{

	if (KEY_INPUT(KEY::V, KEY_STATE::HOLD))
	{
		return true;
	}

	return false;
}

void CCorvusStateBase::OnHit(weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	__super::OnHit(pOtherCollider, In_eHitType, In_fDamage);

	/*if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::MONSTER_ATTACK)
	{
		if (Get_OwnerPlayer()->Get_DodgeTime() > 0.f)
		{
			return;
		}

		if (In_eHitType == HIT_TYPE::NORMAL_HIT)
		{
			_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
				Get_ParentObject().lock()->
				Get_Component<CTransform>().lock()->
				Get_State(CTransform::STATE_TRANSLATION);

			_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
			vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

			m_pTransformCom.lock()->LookAt(vSameHeightOtherColliderPosition);

			Get_OwnerPlayer()->Change_State<CCorvusState_Hit>();
			m_pStatusCom.lock()->Add_Damage(In_fDamage);

		}

		if (In_eHitType == HIT_TYPE::DOWN_HIT)
		{
			_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
				Get_ParentObject().lock()->
				Get_Component<CTransform>().lock()->
				Get_State(CTransform::STATE_TRANSLATION);

			_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
			vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

			m_pTransformCom.lock()->LookAt(vSameHeightOtherColliderPosition);

			Get_OwnerPlayer()->Change_State<CLuxiyaState_HitDown>();
			m_pStatusCom.lock()->Add_Damage(In_fDamage);

		}
	}*/

	

}

void CCorvusStateBase::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pOtherCollider);

}

void CCorvusStateBase::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pOtherCollider);

}

void CCorvusStateBase::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pOtherCollider);

}

void CCorvusStateBase::OnEventMessage(_uint iArg)
{
	if ((_uint)EVENT_TYPE::ON_FIRSTHIT)
	{
		_bool iRandom = (_bool)(rand() % 2);

		if (iRandom)
		{
			GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_Hit.wav"), 1.f);
		}
		else
		{
			GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_Hit2.wav"), 1.f);
		}

	}
}

void CCorvusStateBase::Free()
{
}
