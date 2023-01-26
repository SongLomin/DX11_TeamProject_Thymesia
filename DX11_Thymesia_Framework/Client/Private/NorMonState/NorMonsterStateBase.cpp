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
#include "UI_DamageFont.h"
#include "PhysXCharacterController.h"
#include "Status_Monster.h"

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

	//GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);

	//T_SINGLE(CGameManager)->Use_EffectGroup("Hit_Monster2", m_pTransformCom);
}

void CNorMonsterStateBase::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	__super::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);

	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::PLAYER_ATTACK)
	{
		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		//맞았을때 플레이어를 바라보는 시선 처리
		weak_ptr<CAttackArea> pAttackArea = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner());

		if (!pAttackArea.lock())
			return;

		weak_ptr<CCharacter> pOtherCharacter = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->Get_ParentObject();
		
		weak_ptr<CStatus_Monster> pStatus = m_pOwner.lock()->Get_Component<CStatus_Monster>();

		_vector vOtherColliderPosition = pOtherCharacter.lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

		m_pTransformCom.lock()->LookAt2D(vSameHeightOtherColliderPosition);

		//데미지 적용

		ATTACK_OPTION eAttackOption =  pAttackArea.lock()->Get_OptionType();
		
		//pAttackArea.lock()->Get_ParentObject().lock()->

		CStatus_Player::PLAYERDESC tPlayerDesc;
		PxControllerFilters Filters;

		pAttackArea.lock()->Get_ParentObject().lock()->Get_ComponentByType<CStatus>().lock()
			->Get_Desc(&tPlayerDesc);

		//플레이어 공격력 아직 없으니 임의값 넣어서!
		_float fMagnifiedDamage = In_fDamage;
		//m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
		_vector vViewPosition;
		_matrix ViewProjMatrix;
		_matrix vResultOtherWorldMatrix;

		vViewPosition = pMyCollider.lock()->Get_Owner().lock()->Get_Transform()->Get_Position();
			
		vViewPosition += XMVectorSet(0.f, 2.f, 0.f, 1.f);

		ViewProjMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_VIEW) * GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_PROJ);

		vViewPosition = XMVector3TransformCoord(vViewPosition, ViewProjMatrix);

		/* -1 ~ 1 to 0 ~ ViewPort */
		vViewPosition.m128_f32[0] = (vViewPosition.m128_f32[0] + 1.f) * (_float)g_iWinCX * 0.5f;
		vViewPosition.m128_f32[1] = (-1.f * vViewPosition.m128_f32[1] + 1.f) * (_float)g_iWinCY * 0.5f;

		weak_ptr<CUI_DamageFont> pDamageFont = GAMEINSTANCE->Add_GameObject<CUI_DamageFont>(LEVEL_STATIC);
		
		_float2 vHitPos;
		
		vHitPos.x = vViewPosition.m128_f32[0];
		vHitPos.y = vViewPosition.m128_f32[1];

		random_device rd;
		mt19937_64 mt(rd());

		uniform_real_distribution<_float> fRandom(-50.f, 50.f);

		vHitPos.x += fRandom(mt);
		vHitPos.y += fRandom(mt);

		_int iRand = rand() % 20 + 1;

		switch (eAttackOption)
		{
		case Client::ATTACK_OPTION::NONE:
			fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk + iRand;
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, ATTACK_OPTION::NORMAL);		
			GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
			pDamageFont.lock()->SetUp_DamageFont((_uint)fMagnifiedDamage, vHitPos, eAttackOption);
			break;
		case Client::ATTACK_OPTION::NORMAL:
			fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk + iRand;
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);		
			GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
			pDamageFont.lock()->SetUp_DamageFont((_uint)fMagnifiedDamage, vHitPos, eAttackOption);
			break;
		case Client::ATTACK_OPTION::PLAGUE:
			fMagnifiedDamage *= tPlayerDesc.m_fPlagueAtk + iRand;
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);		
			GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
			pDamageFont.lock()->SetUp_DamageFont((_uint)fMagnifiedDamage, vHitPos, eAttackOption);
			break;
		case Client::ATTACK_OPTION::SPECIAL_ATTACK:		
			if (!m_pStatusCom.lock()->Is_Dead())
			{
				if (In_eHitType == HIT_TYPE::RIGHT_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk + iRand;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerMonster()->Change_State<CNorMonState_HurtR>();
				}
				else if (In_eHitType == HIT_TYPE::LEFT_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk + iRand;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerMonster()->Change_State<CNorMonState_HurtL>();
				}
				else if (In_eHitType == HIT_TYPE::NORMAL_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk + iRand;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerMonster()->Change_State<CNorMonState_HurtL>();
				}
				else if (In_eHitType == HIT_TYPE::DOWN_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
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
				pOtherCharacter.lock()->OnStealMonsterSkill(Get_OwnerMonster()->Get_MonsterType());
				pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_STEALCORVUS);
			}
			else if (In_eHitType == HIT_TYPE::LEFT_HIT)
			{
				Get_OwnerMonster()->Change_State<CNorMonState_HurtL>();
			}
			else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
			{
				Get_OwnerMonster()->Change_State<CNorMonState_HurtR>();
			}
			break;
		}

		_float3 vShakingOffset = pOtherCharacter.lock()->Get_CurState().lock()->Get_ShakingOffset();
		_vector vShakingOffsetToVector = XMLoadFloat3(&vShakingOffset);
		_float fShakingRatio = 0.01f*iRand;

		
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffsetToVector, 0.08f + fShakingRatio, 1.f, 9.f, 0.25f);
		GAMEINSTANCE->Set_MotionBlur(0.05f);

		Get_OwnerMonster()->Set_RimLightDesc(4.5f, { 0.6f,0.f,0.f }, 0.9f);


		GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Set_TargetMonster(Get_OwnerMonster());

		Play_OnHitEffect();

		//공격 형태에 따라서 애니메이션 변경

	

		if (m_eMonType != MONSTERTYPE::BALLOON)
		{
			if (Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_GroggyLoop>().lock()->Get_StateIndex()
				|| Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_GroggyStart>().lock()->Get_StateIndex())
			{
				if (m_eMonType != MONSTERTYPE::WEAKARMORSPEARMAN && m_eMonType != MONSTERTYPE::ARMORSPEARMAN)
				{
					Get_OwnerMonster()->Change_State<CNorMonState_Die>();
				}		    
				else
				{
				
					if (eAttackOption == ATTACK_OPTION::NONE ||
						eAttackOption == ATTACK_OPTION::NORMAL)
					{
						if (m_eMonType == MONSTERTYPE::WEAKARMORSPEARMAN || m_eMonType == MONSTERTYPE::ARMORSPEARMAN)
						{
							pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_ARMOREXECUTIONSPEAR);
						}
					}
					
					
				}
			}
			else if (m_pStatusCom.lock()->Is_Dead())
			{
				Get_OwnerMonster()->Change_State<CNorMonState_GroggyStart>();
			}
			else if (Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Idle>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Run>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Walk_F>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Walk_L>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Walk_R>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Walk_B>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_TurnL90>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_TurnR90>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_HurtR>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_HurtL>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_Awake>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CNorMonState_SitToIdle>().lock()->Get_StateIndex())

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
		else
		{
			if (m_pStatusCom.lock()->Is_Dead())
			{
				Get_OwnerMonster()->Change_State<CNorMonState_Die>();
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

}

void CNorMonsterStateBase::Free()
{
}
