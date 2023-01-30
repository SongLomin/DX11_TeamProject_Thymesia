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
#include "Status_Boss.h"
#include "Monster.h"
#include "BossUrd/UrdStates.h"
#include "PhysXCharacterController.h"
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

void CUrdBossStateBase::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	__super::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);

	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::PLAYER_ATTACK)
	{
		_uint iRandom = rand() % 3;

		switch (iRandom)
		{
		case 0: GAMEINSTANCE->PlaySound3D("Impact_Blade_Metal_Medium_Armor_Flesh_03.wav", 1.f, m_pTransformCom.lock()->Get_Position()); break;
		case 1: GAMEINSTANCE->PlaySound3D("Impact_Blade_Metal_Medium_Armor_Flesh_04.wav", 1.f, m_pTransformCom.lock()->Get_Position()); break;
		case 2: GAMEINSTANCE->PlaySound3D("Impact_Blade_Metal_Medium_Armor_Flesh_05.wav", 1.f, m_pTransformCom.lock()->Get_Position()); break;
		}

		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		//맞았을때 플레이어를 바라보는 시선 처리
		weak_ptr<CAttackArea> pAttackArea = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner());

		weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();

		weak_ptr<CCharacter> pOtherCharacter = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->Get_ParentObject();

		weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();


		if (!pAttackArea.lock())
			return;

		_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];
		PxControllerFilters Filters;

		m_pTransformCom.lock()->LookAt2D(vSameHeightOtherColliderPosition);

		ATTACK_OPTION eAttackOption = pAttackArea.lock()->Get_OptionType();


		CStatus_Player::PLAYERDESC tPlayerDesc;
		_matrix                    vResultOtherWorldMatrix;
		//_matrix                    vDoorOpenPlayerMatrix = Get_Owner().lock()->Get_Component<CVargBossState_Start>().lock()->Get_PlayerTransform();

		pAttackArea.lock()->Get_ParentObject().lock()->Get_ComponentByType<CStatus>().lock()
			->Get_Desc(&tPlayerDesc);


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
					Get_OwnerMonster()->Change_State<CUrdBossState_HurtM_FR>();
				}

				else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerMonster()->Change_State<CUrdBossState_HurtM_FL>();
				}
				else if (In_eHitType == HIT_TYPE::DOWN_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerMonster()->Change_State<CUrdBossState_HurtM_FL>();
				}
				else if (In_eHitType == HIT_TYPE::NORMAL_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerMonster()->Change_State<CUrdBossState_HurtM_FL>();
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
				Get_OwnerMonster()->Change_State<CUrdBossState_HurtM_FR>();
			}
			else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
			{
				GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->OnStealMonsterSkill(Get_OwnerMonster()->Get_MonsterType());
				Get_OwnerMonster()->Change_State<CUrdBossState_HurtM_FL>();
			}
			break;
		}
		_float3 vShakingOffset = pOtherCharacter.lock()->Get_CurState().lock()->Get_ShakingOffset();
		_vector vShakingOffsetToVector = XMLoadFloat3(&vShakingOffset);
		_float fShakingRatio = 0.01f * iRand;

		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffsetToVector, 0.08f + fShakingRatio, 1.f, 9.f, 0.25f);
		GAMEINSTANCE->Set_MotionBlur(0.05f);

		Get_OwnerMonster()->Set_RimLightDesc(4.5f, { 0.6f,0.f,0.f }, 0.9f);

		//1,2 ,3 패턴
		//보스1페이지랑 2페이지 달르게
		
		_uint iLifeCount = pStatus.lock()->Get_Desc().m_iLifeCount;

		_float	fGreenRatio = 0.f;
		_uint	iSkillCount = Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Get_SkillCount();

		if (iLifeCount == 2)
		{
			fGreenRatio = pStatus.lock()->Get_GreenRatio();



			if (fGreenRatio <= 0.75f && iSkillCount == 0)
			{
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(true);
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillCount(1);
			}
			else if (fGreenRatio <= 0.5f && iSkillCount == 1)
			{
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(true);
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillCount(2);
			}
			else if (fGreenRatio <= 0.25f && iSkillCount == 2)
			{
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(true);
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillCount(3);
			}

		}
		else
		{
			Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_PhaseTwoSkillCount(1);	


			if (Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Get_PhaseTwoSkillCount() >= 6)
			{
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(true);
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_ZeroPhaseTwoSkillCount(0);

			}



			if (Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Get_PhaseTwoJavlinCount() >= 3)
			{
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SpecailAttack(true);
				Get_OwnerMonster()->Change_State<CUrdBossState_Idle>();
				return;
			}




		}

		if (Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Get_SkillStart())
		{
			int iRand = rand() % 2;
			switch (iRand)
			{
			case 0:
				Get_OwnerMonster()->Change_State<CUrdBossState_Parry_R>();
				pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_PARRYRIGHT);

				break;
			case 1:
				Get_OwnerMonster()->Change_State<CUrdBossState_Parry_L>();
				pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_PARRYLEFT);
				break;
			}
			return;
		}

	

		
	
	

		if (Get_OwnerCharacter().lock()->Get_CurState().lock() != Get_Owner().lock()->Get_Component<CUrdBossState_StunStart>().lock() &&
			Get_OwnerCharacter().lock()->Get_CurState().lock() != Get_Owner().lock()->Get_Component<CUrdBossState_StunLoop>().lock())
		{
			if (pStatus.lock()->Is_Dead())
			{
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StunStart>(0.05f);
			}
		}
		else
		{
			//이떄 플레이어한테 이벤트를 던져줍시다
			if (pStatus.lock()->Get_Desc().m_iLifeCount == 2)
			{
				if (eAttackOption == ATTACK_OPTION::NORMAL)
				pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_URDEXECUTON);
			}
			else
			{
				if (eAttackOption == ATTACK_OPTION::NORMAL)
				pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_URDEXECUTON);
								
			}


		}




		if (!pStatus.lock()->Is_Dead())
		{
			if (Get_StateIndex() == m_pOwner.lock()->Get_Component<CUrdBossState_Idle>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CUrdBossState_HurtS_FR>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CUrdBossState_HurtS_FL>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CUrdBossState_WalkL>().lock()->Get_StateIndex() ||
				Get_StateIndex() == m_pOwner.lock()->Get_Component<CUrdBossState_WalkR>().lock()->Get_StateIndex() )		
			{

				if (In_eHitType == HIT_TYPE::LEFT_HIT)
				{
					if (Get_Owner().lock()->Get_Component<CUrdBossState_HurtS_FL>().lock()->Get_ParryCount() >= 3 ||
						Get_Owner().lock()->Get_Component<CUrdBossState_HurtS_FR>().lock()->Get_ParryCount() >= 3)
					{
						Get_OwnerMonster()->Change_State<CUrdBossState_Parry_L>();
						return;
					}			
					Get_OwnerMonster()->Change_State<CUrdBossState_HurtS_FR>();
					return;
				
				}

				else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
				{
					if (Get_Owner().lock()->Get_Component<CUrdBossState_HurtS_FL>().lock()->Get_ParryCount() >= 3 ||
						Get_Owner().lock()->Get_Component<CUrdBossState_HurtS_FR>().lock()->Get_ParryCount() >= 3)
					{
						Get_OwnerMonster()->Change_State<CUrdBossState_Parry_R>();
						return;
					}
					Get_OwnerMonster()->Change_State<CUrdBossState_HurtS_FL>();
					return;
					
				}
			}
		}
	
		  
	}


}

void CUrdBossStateBase::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);

}

void CUrdBossStateBase::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);

}

void CUrdBossStateBase::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);

}

void CUrdBossStateBase::OnEventMessage(_uint iArg)
{
	
}

void CUrdBossStateBase::Free()
{
}
