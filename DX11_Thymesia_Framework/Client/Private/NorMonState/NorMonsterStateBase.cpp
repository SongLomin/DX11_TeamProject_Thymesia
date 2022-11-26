#include "stdafx.h"
#include "NorMonState/NorMonsterStateBase.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Weapon.h"
#include "GameManager.h"
#include "Player.h"
#include "NorMonStates.h"
//#include "MonsterHPBar.h"
#include "Status.h"
//#include "ComboTimer.h"
#include "Attack_Area.h"
//#include "DamageUI.h"
#include "Status_Player.h"

GAMECLASS_C(CNorMonsterStateBase)

_bool CNorMonsterStateBase::Check_RequirementAttackState()
{
	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return true;
	}

	return false;
}

_bool CNorMonsterStateBase::Check_RequirementDashState()
{
	if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		return true;
	}
	return false;
}

_bool CNorMonsterStateBase::Check_RequirementRunState()
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

_bool CNorMonsterStateBase::Check_RequirementPlayerInRange(const _float& In_fRange)
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	_vector vPlayerPosition = pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVector3Length(vPlayerPosition - vMyPosition).m128_f32[0];

	return In_fRange >= fDistance;
}

void CNorMonsterStateBase::Play_OnHitEffect()
{
	//_vector vLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	//vLook *= -1.f;

	//_matrix ReverseLookMatrix = SMath::Bake_MatrixNormalizeUseLookVector(vLook);
	//ReverseLookMatrix.r[3] = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

	GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);

	//T_SINGLE(CGameManager)->Use_EffectGroup("Hit_Monster2", m_pTransformCom);
}

void CNorMonsterStateBase::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	//__super::OnHit(pOtherCollider, In_eHitType, In_fDamage);

	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::PLAYER_ATTACK)
	{
		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		//맞았을때 플레이어를 바라보는 시선 처리
		weak_ptr<CAttackArea> pAttackArea = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner());

		if (!pAttackArea.lock())
			return;

		weak_ptr<CCharacter> pOtherCharacter = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->Get_ParentObject();
		
		_float3 vShakingOffset = pOtherCharacter.lock()->Get_CurState().lock()->Get_ShakingOffset();
		_vector vShakingOffsetToVector = XMLoadFloat3(&vShakingOffset);

		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffsetToVector, 0.5f, 0.1f);//일반 공격

		_vector vOtherColliderPosition = pOtherCharacter.lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

		m_pTransformCom.lock()->LookAt(vSameHeightOtherColliderPosition);

		//데미지 적용

		ATTACK_OPTION eAttackOption =  pAttackArea.lock()->Get_OptionType();
		
		//pAttackArea.lock()->Get_ParentObject().lock()->

		CStatus_Player::PLAYERDESC tPlayerDesc;

		pAttackArea.lock()->Get_ParentObject().lock()->Get_ComponentByType<CStatus>().lock()
			->Get_Desc(&tPlayerDesc);

		//플레이어 공격력 아직 없으니 임의값 넣어서!
		_float fMagnifiedDamage = In_fDamage * 4;
		//m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);

		switch (eAttackOption)
		{
		case Client::ATTACK_OPTION::NONE:
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, ATTACK_OPTION::NORMAL);		
			break;
		case Client::ATTACK_OPTION::NORMAL:
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);		
			break;
		case Client::ATTACK_OPTION::PLAGUE:
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);		
			break;
		case Client::ATTACK_OPTION::SPECIAL_ATTACK:
			break;
		}
		//GAMEINSTANCE->Get_GameObjects<CDamageUI>(LEVEL::LEVEL_STATIC).front().lock()->Add_DamageText(vMyPosition, In_fDamage, bRandom);

		//GAMEINSTANCE->Get_GameObjects<CMonsterHpBar>(LEVEL::LEVEL_STATIC).front().lock()->OnHit(m_pOwner);
		//GAMEINSTANCE->Get_GameObjects<CComboTimer>(LEVEL::LEVEL_STATIC).front().lock()->Update_Combo();

	/*	switch (eAttackOption)
		{
		case Client::ATTACK_OPTION::NONE:
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, ATTACK_OPTION::NORMAL);
			break;
		case Client::ATTACK_OPTION::NORMAL:
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
			break;
		case Client::ATTACK_OPTION::PLAGUE:
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
			break;
		case Client::ATTACK_OPTION::SPECIAL_ATTACK:
			break;
		}*/

		GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Set_TargetMonster(Get_OwnerMonster());

		Play_OnHitEffect();

		//공격 형태에 따라서 애니메이션 변경

		
		if (Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_GroggyLoop>().lock()->Get_StateIndex()
			|| Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_GroggyStart>().lock()->Get_StateIndex())
		{
			Get_OwnerMonster()->Change_State<CNorMonState_Die>();
		}
		else if (m_pStatusCom.lock()->Is_Dead())
		{
			Get_OwnerMonster()->Change_State<CNorMonState_GroggyStart>();
		}	
		else if (Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Idle>().lock()->Get_StateIndex()||
			Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Run>().lock()->Get_StateIndex()||
			Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Walk_F>().lock()->Get_StateIndex()||
			Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Walk_L>().lock()->Get_StateIndex()||
			Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Walk_R>().lock()->Get_StateIndex()||
			Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Walk_B>().lock()->Get_StateIndex()||
			Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_TurnL90>().lock()->Get_StateIndex()||
			Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_TurnR90>().lock()->Get_StateIndex())
		{
			if (In_eHitType == HIT_TYPE::LEFT_HIT)
			{
				Get_OwnerMonster()->Change_State<CNorMonState_HurtL>();
			}

			else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
			{
				Get_OwnerMonster()->Change_State<CNorMonState_HurtR>();
			}
		}
	}

}





void CNorMonsterStateBase::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);

}

void CNorMonsterStateBase::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);

}

void CNorMonsterStateBase::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);

}

void CNorMonsterStateBase::OnEventMessage(_uint iArg)
{
	GAMEINSTANCE->PlaySoundW(TEXT("Monstet1_Hit.wav"), 1.f);
}

void CNorMonsterStateBase::Free()
{
}
