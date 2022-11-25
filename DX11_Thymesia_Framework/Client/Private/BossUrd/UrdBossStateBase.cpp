#include "stdafx.h"
#include "BossUrd/UrdBossStateBase.h"
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


GAMECLASS_C(CUrdBossStateBase)

_bool CUrdBossStateBase::Check_RequirementAttackState()
{
	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return true;
	}

	return false;
}

_bool CUrdBossStateBase::Check_RequirementDashState()
{
	if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		return true;
	}
	return false;
}

_bool CUrdBossStateBase::Check_RequirementRunState()
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

_bool CUrdBossStateBase::Check_RequirementPlayerInRange(const _float& In_fRange)
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	_vector vPlayerPosition = pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVector3Length(vPlayerPosition - vMyPosition).m128_f32[0];

	return In_fRange >= fDistance;
}

void CUrdBossStateBase::Play_OnHitEffect()
{
	/*_vector vLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	vLook *= -1.f;

	_matrix ReverseLookMatrix = SMath::Bake_MatrixNormalizeUseLookVector(vLook);
	ReverseLookMatrix.r[3] = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

	GET_SINGLE(CGameManager)->Use_EffectGroup("Hit_Monster1", ReverseLookMatrix);*/

	GET_SINGLE(CGameManager)->Use_EffectGroup("Hit_Monster2", m_pTransformCom);

}

void CUrdBossStateBase::OnHit(weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	__super::OnHit(pOtherCollider, In_eHitType, In_fDamage);

	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::PLAYER_ATTACK)
	{
		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		//맞았을때 플레이어를 바라보는 시선 처리
		weak_ptr<CAttackArea> pAttackArea = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner());

		if (!pAttackArea.lock())
			return;

		_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		/*_vector vOtherColliderPosition = Weak_Cast<CWeapon>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);*/

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

		m_pTransformCom.lock()->LookAt(vSameHeightOtherColliderPosition);

		_bool bRandom = (_bool)(rand() % 2);

		//데미지 적용
		//m_pStatusCom.lock()->Add_Damage(In_fDamage);
		//GAMEINSTANCE->Get_GameObjects<CDamageUI>(LEVEL::LEVEL_STATIC).front().lock()->Add_DamageText(vMyPosition, In_fDamage, bRandom);

		//GAMEINSTANCE->Get_GameObjects<CMonsterHpBar>(LEVEL::LEVEL_STATIC).front().lock()->OnHit(m_pOwner);
		//GAMEINSTANCE->Get_GameObjects<CComboTimer>(LEVEL::LEVEL_STATIC).front().lock()->Update_Combo();

		GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Set_TargetMonster(Get_OwnerMonster());

		Play_OnHitEffect();

		//공격 형태에 따라서 애니메이션 변경

		//if (m_pStatusCom.lock()->Is_Dead())
		//{
		//	Get_OwnerMonster()->Change_State<CMonster1State_Death>();
		//}

		//else if (In_eHitType == HIT_TYPE::NORMAL_HIT)
		//{
		//	Get_OwnerMonster()->Change_State<CMonster1State_Hit>();
		//	GET_SINGLE(CGameManager)->Add_Shaking(SHAKE_DIRECTION::LOOK, 0.15f, 0.2f);
		//}
		//
		//else if (In_eHitType == HIT_TYPE::DOWN_HIT)
		//{
		//	Get_OwnerMonster()->Change_State<CMonster1State_HitDown>(0.1f);
		//	GET_SINGLE(CGameManager)->Add_Shaking(SHAKE_DIRECTION::RIGHT, 0.25f, 0.3f);
		//}


	}

}

void CUrdBossStateBase::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pOtherCollider);

}

void CUrdBossStateBase::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pOtherCollider);

}

void CUrdBossStateBase::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pOtherCollider);

}

void CUrdBossStateBase::OnEventMessage(_uint iArg)
{
	GAMEINSTANCE->PlaySoundW(TEXT("Monstet1_Hit.wav"), 1.f);
}

void CUrdBossStateBase::Free()
{
}
