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

_bool CCorvusStateBase::Check_RequirementExcuteState(weak_ptr<CGameObject>& Out_pGameObject)
{
	list<weak_ptr<CGameObject>>  pGameObjects = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::GROOGYMOSNTER);


	//몬스터랑 거리비교를해야함

	return Get_NearGameObjectInDistance(Out_pGameObject, pGameObjects,5.f);
}

void CCorvusStateBase::OnHit(weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	__super::OnHit(pOtherCollider, In_eHitType, In_fDamage);

	/*
		공격을 받았다!
		//공격을받았다 온히트들어온게공격받은거임
		
		패링 상태인가?
			->Yes 퍼펙트 패링인가?
					->->Yes : 데미지 무시후 패링데미지 존나가함
					->->NO : 노말 패링인가?
							->->->Yes : 데미지 무시 후 패링데미지 가함.
							->->->NO : 그냥 맞아라 너는(피격)
			->NO : 데미지 가하고, 죽었는가?
					->->Yes : 데드씬 발생.
					->->NO : 피격
	
	*/


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

		if (Get_OwnerCharacter().lock()->Get_CurState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Parry1>().lock() ||
			Get_OwnerCharacter().lock()->Get_CurState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Parry2>().lock())
		{
			/*
				지금 맞았을 당시 상태가 패링이라면
			*/
			pMonsterStatusCom.lock()->Decrease_ParryGauge(40.f);
			//수민이형 이거 애들 체력 다나가도 Hit상태로 안가요
			//이거좀 어케 해줘요

			_bool bGroggy = pMonsterStatusCom.lock()->Is_Groggy();

			if (bGroggy)
			{
				pMonsterFromCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_GROGGY);
			}


			PARRY_TYPE eParryingType = Get_ParryType();
			int a = 10;
			switch (eParryingType)
			{
			case Client::PARRY_TYPE::PERFECT:
				//이떄그럼 몬스터 패링게이지 존나깍으면됨 
				a = 10;
				return;
			case Client::PARRY_TYPE::NORMAL:
				// 이떄 플레이어데미지감소시키지는말고 패링게이지 조금증가
				a= 17;
				return;
			case Client::PARRY_TYPE::FAIL:
				// 걍 플레이어처맞으면될듯
				a = 5;
				break;
			case Client::PARRY_TYPE::NONE:
				//이건뭐냐
				a = 6;
				break;
			}
			
		}
	
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
		pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);
		
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
	/*if ((_uint)EVENT_TYPE::ON_EXCUTION_NORMOB)
	{
		Get_OwnerCharacter().lock()->Change_State<CNorMob_Execution>();
	}*/
}

void CCorvusStateBase::Free()
{
}
