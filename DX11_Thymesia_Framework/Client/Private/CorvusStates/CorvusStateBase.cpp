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
	if (KEY_INPUT(KEY::RBUTTON, KEY_STATE::TAP))
	{
		return true;
	}

	return false;
}

void CCorvusStateBase::OnHit(weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	__super::OnHit(pOtherCollider, In_eHitType, In_fDamage);

	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::MONSTER_ATTACK)
	{
		//어쩃든 여기 닿으면 데미지 입음.
		weak_ptr<CStatus_Player> pStatus = Weak_StaticCast<CStatus_Player>(m_pStatusCom);

		if (!pStatus.lock())
		{
			MSG_BOX("Error: Can't Find CStatus_Player From CorvusStateBase!");
		}

		weak_ptr<CAttackArea>	pAttackArea = Weak_StaticCast<CAttackArea>(pOtherCollider.lock()->Get_Owner());
		weak_ptr<CCharacter>	pMonsterFromCharacter = pAttackArea.lock()->Get_ParentObject();
		weak_ptr<CStatus_Monster>	pMonsterStatusCom = pMonsterFromCharacter.lock()->Get_Component<CStatus_Monster>();
				
		if(!pMonsterStatusCom.lock())
			MSG_BOX("Error : Can't Find CStatus_Monster From CorvusStateBase");

		pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);

		/*if (pMonsterFromCharacter.lock()->Get_CurState().lock()->G == )
		{
			Get_OwnerPlayer()->Change_State<CNorMob_Execution>();
		}*/
		//뎀지가 까인 직후 테스트
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


			Get_OwnerPlayer()->Change_State<CCorvusState_HurtL>();
			//m_pStatusCom.lock()->Add_Damage(In_fDamage);
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
			//m_pStatusCom.lock()->Add_Damage(In_fDamage);
		}
	}
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
	if ((_uint)EVENT_TYPE::ON_EXCUTION_NORMOB)
	{
		Get_OwnerCharacter().lock()->Change_State<CNorMob_Execution>();
	}
}

void CCorvusStateBase::Free()
{
}
