#include "stdafx.h"
#include "VargBossStateBase.h"
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
#include "VargStates.h"
#include "PhysXCharacterController.h"
#include "Status_Boss.h"

GAMECLASS_C(CVargBossStateBase)

_bool CVargBossStateBase::Check_RequirementAttackState()
{
	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
		return true;

	return false;
}

_bool CVargBossStateBase::Check_RequirementDashState()
{
	if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
		return true;

	
	return false;
}

_bool CVargBossStateBase::Check_RequirementRunState()
{
	if (KEY_INPUT(KEY::W, KEY_STATE::HOLD)
		|| KEY_INPUT(KEY::A, KEY_STATE::HOLD)
		|| KEY_INPUT(KEY::S, KEY_STATE::HOLD)
		|| KEY_INPUT(KEY::D, KEY_STATE::HOLD))
		return true;
	

	return false;
}

_uint CVargBossStateBase::Check_RequirementDotState()
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

_bool CVargBossStateBase::Check_RequirementPlayerInRange(const _float& In_fRange)
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	_vector vPlayerPosition = pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVector3Length(vPlayerPosition - vMyPosition).m128_f32[0];

	return In_fRange >= fDistance;
}

void CVargBossStateBase::Play_OnHitEffect()
{
	/*_vector vLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	vLook *= -1.f;

	_matrix ReverseLookMatrix = SMath::Bake_MatrixNormalizeUseLookVector(vLook);
	ReverseLookMatrix.r[3] = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

	GET_SINGLE(CGameManager)->Use_EffectGroup("Hit_Monster1", ReverseLookMatrix);*/

	GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);

	GET_SINGLE(CGameManager)->Use_EffectGroup("Hit_Monster2", m_pTransformCom);
}

void CVargBossStateBase::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	__super::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);

	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::PLAYER_ATTACK)
	{
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
		_matrix                    vDoorOpenPlayerMatrix = Get_Owner().lock()->Get_Component<CVargBossState_Start>().lock()->Get_PlayerTransform();

		pAttackArea.lock()->Get_ParentObject().lock()->Get_ComponentByType<CStatus>().lock()
			->Get_Desc(&tPlayerDesc);
		

		Play_OnHitEffect();

		_float fMagnifiedDamage = In_fDamage;
		_uint iRand = rand() % 20 + 1;

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
				}
					
				else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
				}

				else if (In_eHitType == HIT_TYPE::DOWN_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
				}
				else if (In_eHitType == HIT_TYPE::NORMAL_HIT)
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
				pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_STEALCORVUS);
			}
			else if (In_eHitType == HIT_TYPE::LEFT_HIT)
			{
				Get_OwnerMonster()->Change_State<CVargBossState_Hurt>();
			}
			else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
			{
				GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->OnStealMonsterSkill(Get_OwnerMonster()->Get_MonsterType());
				Get_OwnerMonster()->Change_State<CVargBossState_Hurt>();
			}
			break;
		default:
			fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
			m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
			break;
		}
		_float3 vShakingOffset = pOtherCharacter.lock()->Get_CurState().lock()->Get_ShakingOffset();
		_vector vShakingOffsetToVector = XMLoadFloat3(&vShakingOffset);
		_float fShakingRatio = 0.01f * iRand;


		//이거는한번만호출되게 해야함 
		//현재상태가 스턴스타트나 루프가아닌경우
		//혹시몰르니 예외처리해줌 
		if (Get_OwnerCharacter().lock()->Get_CurState().lock() != Get_Owner().lock()->Get_Component<CVargBossState_Stun_Start>().lock() &&
			Get_OwnerCharacter().lock()->Get_CurState().lock() != Get_Owner().lock()->Get_Component<CVargBossState_Stun_Loop>().lock())
		{
			if (pStatus.lock()->Is_Dead())
			{
				Get_OwnerCharacter().lock()->Change_State<CVargBossState_Stun_Start>(0.05f);		
			}
		}
		else
		{
			//이떄 플레이어한테 이벤트를 던져줍시다
			if (pStatus.lock()->Get_Desc().m_iLifeCount == 2)
			{			
				if (eAttackOption == ATTACK_OPTION::NONE ||
					eAttackOption == ATTACK_OPTION::NORMAL)

				{
					pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_VARGEXECUTION);


					_matrix vOtherWorldMatrix = pCurrentPlayer.lock()->Get_Transform()->Get_WorldMatrix();
					vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, 0.f, -1.4f, 0.f));
					pOtherCharacter.lock()->Get_PhysX().lock()->Set_Position(
						vResultOtherWorldMatrix.r[3],
						GAMEINSTANCE->Get_DeltaTime(),
						Filters);


					Get_Owner().lock()->Get_Component<CVargBossState_Exe_Start>().lock()->Set_DieType(true);
					Get_Owner().lock()->Get_Component<CVargBossState_Exe_NoDeadEnd>().lock()->Set_DeadChoice(true);
				}
							
			}
			else
			{
				if (eAttackOption == ATTACK_OPTION::NONE ||
					eAttackOption == ATTACK_OPTION::NORMAL)
				{
					pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_VARGEXECUTION);


					_matrix vOtherWorldMatrix = pCurrentPlayer.lock()->Get_Transform()->Get_WorldMatrix();
					vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, 0.f, -1.4f, 0.f));
					pOtherCharacter.lock()->Get_PhysX().lock()->Set_Position(
						vResultOtherWorldMatrix.r[3],
						GAMEINSTANCE->Get_DeltaTime(),
						Filters);

					Get_Owner().lock()->Get_Component<CVargBossState_Exe_NoDeadEnd>().lock()->Set_DeadChoice(true);
					Get_Owner().lock()->Get_Component<CVargBossState_Exe_Start>().lock()->Set_DieType(false);

				}
			}
		}
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffsetToVector, 0.1f + fShakingRatio, 1.f, 9.f, 0.5f);//일반 공격
		GAMEINSTANCE->Set_MotionBlur(0.05f);

		Get_OwnerMonster()->Set_RimLightDesc(4.5f, { 0.6f,0.f,0.f }, 0.9f);
		//현재상태가 스턴스타트나 스턴루프인경우에 
		//다시 검사를해준다 플레이어의 공격이 들어오면 바그처형으로 갑니다 
		// 바그처형으로가고 바그처형으로 갈떄 그 애니메이션한태 값하나던져주면 해결ㅇ완료 
	}
}

void CVargBossStateBase::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);

}

void CVargBossStateBase::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);

}

void CVargBossStateBase::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);

}

void CVargBossStateBase::OnEventMessage(_uint iArg)
{
	
}

void CVargBossStateBase::OnEventMessage(weak_ptr<CBase> pArg)
{
}

void CVargBossStateBase::Free()
{
}

void CVargBossStateBase::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
}

void CVargBossStateBase::OnStateEnd()
{
	__super::OnStateEnd();

	m_bShakingCamera = false;
}
