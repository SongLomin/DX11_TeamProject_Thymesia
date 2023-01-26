#include "stdafx.h"
#include "BossBat/BatBossStateBase.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Weapon.h"
#include "GameManager.h"
#include "Player.h"
#include "BoneNode.h"
//#include "MonsterHPBar.h"
#include "Status.h"
//#include "ComboTimer.h"
#include "Attack_Area.h"
#include "BossBat/BatStates.h"
//#include "DamageUI.h"
#include "PhysXCharacterController.h"
#include "Status_Boss.h"

GAMECLASS_C(CBatBossStateBase)

_bool CBatBossStateBase::Check_RequirementAttackState()
{
	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return true;
	}

	return false;
}

_bool CBatBossStateBase::Check_RequirementDashState()
{
	if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		return true;
	}
	return false;
}

_bool CBatBossStateBase::Check_RequirementRunState()
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

_bool CBatBossStateBase::Check_RequirementPlayerInRange(const _float& In_fRange)
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	_vector vPlayerPosition = pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVector3Length(vPlayerPosition - vMyPosition).m128_f32[0];

	return In_fRange >= fDistance;
}

void CBatBossStateBase::Play_OnHitEffect()
{
	/*_vector vLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	vLook *= -1.f;

	_matrix ReverseLookMatrix = SMath::Bake_MatrixNormalizeUseLookVector(vLook);
	ReverseLookMatrix.r[3] = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

	GET_SINGLE(CGameManager)->Use_EffectGroup("Hit_Monster1", ReverseLookMatrix);*/

	GET_SINGLE(CGameManager)->Use_EffectGroup("Hit_Monster2", m_pTransformCom);

}

_matrix CBatBossStateBase::Get_LeftHandCombinedWorldMatrix()
{
	_float4x4 ModelMatrix = m_pModelCom.lock()->Get_TransformationMatrix();

	_matrix CombinedMatrix = m_pLeftHandBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&ModelMatrix)
		* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

	return CombinedMatrix;
}

_matrix CBatBossStateBase::Get_RightHandCombinedWorldMatrix()
{
	_float4x4 ModelMatrix = m_pModelCom.lock()->Get_TransformationMatrix();

	_matrix CombinedMatrix = m_pRightHandBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&ModelMatrix)
		* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

	return CombinedMatrix;
}

_matrix CBatBossStateBase::Get_HeadCombinedWorldMatrix()
{
	_float4x4 ModelMatrix = m_pModelCom.lock()->Get_TransformationMatrix();

	_matrix CombinedMatrix = m_pHeadBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&ModelMatrix)
		* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

	return CombinedMatrix;
}

_matrix CBatBossStateBase::Get_ChestCombinedWorldMatrix()
{
	_float4x4 ModelMatrix = m_pModelCom.lock()->Get_TransformationMatrix();

	_matrix CombinedMatrix = m_pChestBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&ModelMatrix)
		* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

	return CombinedMatrix;
}

_matrix CBatBossStateBase::Get_RightFootCombinedWorldMatrix()
{
	_float4x4 ModelMatrix = m_pModelCom.lock()->Get_TransformationMatrix();

	_matrix CombinedMatrix = m_pRightFootBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&ModelMatrix)
		* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

	return CombinedMatrix;
}

_matrix CBatBossStateBase::Get_LeftFootCombinedWorldMatrix()
{
	_float4x4 ModelMatrix = m_pModelCom.lock()->Get_TransformationMatrix();

	_matrix CombinedMatrix = m_pLeftFootBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&ModelMatrix)
		* m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

	return CombinedMatrix;
}

_bool CBatBossStateBase::Check_CrossAttackState()
{
	switch (ComputeDirectionToPlayer())
	{
	case 1:
		Get_OwnerCharacter().lock()->Change_State<CBatBossState_Atk_R01_1>(0.05f);
		break;
	case -1:
		Get_OwnerCharacter().lock()->Change_State<CBatBossState_Atk_L01_1>(0.05f);
		break;
	}

	return true;

}

_bool CBatBossStateBase::Check_CrossJumpState()
{
	switch (ComputeDirectionToPlayer())
	{
	case 1:
		Get_OwnerCharacter().lock()->Change_State<CBatBossState_FTurnL>(0.05f);
		break;
	case -1:
		Get_OwnerCharacter().lock()->Change_State<CBatBossState_FTurnR>(0.05f);
		break;
	}

	return true;

}

_int CBatBossStateBase::Check_DotAttackState()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	CORSS_RESULT CorssResult = CORSS_RESULT::CORSS_END;


	BATATTACK_DOTRESULT DotResult = BATATTACK_DOTRESULT::BATATTACK_DOTRESULT_END;

	_vector vCorssPotision = pCurrentPlayer.lock()->Get_Transform()->Get_Position();
	vCorssPotision.m128_f32[1] = 0.f;
	_vector vOtherDoCorssPositon = m_pOwner.lock()->Get_Transform()->Get_Position();
	vOtherDoCorssPositon.m128_f32[1] = 0.f;
	_vector vPlayerColToMonsterColDireciton = XMVector3Normalize(vOtherDoCorssPositon - vCorssPotision);

	_vector vMyColiderLook = m_pOwner.lock()->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	vMyColiderLook = XMVector3Normalize(vMyColiderLook);

	_vector fCross = XMVector3Cross(vMyColiderLook, vPlayerColToMonsterColDireciton);
	_float  fYCross = XMVectorGetZ(XMVector3Cross(vMyColiderLook, fCross));
	//_float fDir = XMVector3Dot(fCross, XMVectorSet(0.f, 1.f, 0.f, 0.f)).m128_f32[0];

	if (fYCross > 0.f) //양수 오른쪽
	{
		CorssResult = CORSS_RESULT::RIGHT;
	}
	else // 음수
	{
		CorssResult = CORSS_RESULT::LEFT;
	}


	_vector vMyDotPositon = pCurrentPlayer.lock()->Get_Transform()->Get_Position();
	vMyDotPositon.m128_f32[1] = 0.f;
	_vector vOtherDotPositon = m_pOwner.lock()->Get_Transform()->Get_Position();
	vOtherDotPositon.m128_f32[1] = 0.f;

	_vector vOtherColliderToPlayerClollider = XMVector3Normalize(vMyDotPositon - vOtherDotPositon);

	_vector vMyLookVecTor = m_pOwner.lock()->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	vMyLookVecTor.m128_f32[1] = 0;
	vMyLookVecTor = XMVector3Normalize(vMyLookVecTor);

	_float fCos = XMVectorGetX(XMVector3Dot(vOtherColliderToPlayerClollider, vMyLookVecTor));


	//왼쪽오른쪽구분을먼저하고
	//그리고 가운데랑왼쪽 둘이구분해야도미 

	//왼쪽

	if (CorssResult == CORSS_RESULT::LEFT)
	{
		if (fCos  < 0.93969262f)
		{
			return (_uint)BATATTACK_DOTRESULT::LEFT;
		}
		else
		{
			return (_uint)BATATTACK_DOTRESULT::MID;
		}
	}
	else if (CorssResult == CORSS_RESULT::RIGHT)
	{
		if (fCos < 0.93969262f)
		{
			return (_uint)BATATTACK_DOTRESULT::RIGHT;
		}
		else
		{
			return (_uint)BATATTACK_DOTRESULT::MID;
		}
	}
	

		return  (_uint)BATATTACK_DOTRESULT::BATATTACK_DOTRESULT_END;
}



void CBatBossStateBase::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	__super::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);

	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::PLAYER_ATTACK)
	{
		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		//맞았을때 플레이어를 바라보는 시선 처리
		weak_ptr<CAttackArea> pAttackArea = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner());

		weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();

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
		_matrix                    vResultOtherWorldMatrix;

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
					Get_OwnerMonster()->Change_State<CBatBossState_HurtXL_L>();
				}

				else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerMonster()->Change_State<CBatBossState_HurtXL_F>();
				}

				else if (In_eHitType == HIT_TYPE::DOWN_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerMonster()->Change_State<CBatBossState_HurtXL_F>();
				}
				else if (In_eHitType == HIT_TYPE::NORMAL_HIT)
				{
					fMagnifiedDamage *= tPlayerDesc.m_fNormalAtk;
					m_pStatusCom.lock()->Add_Damage(fMagnifiedDamage, eAttackOption);
					Get_OwnerMonster()->Change_State<CBatBossState_HurtXL_F>();
				}
			}	
			break;
		case Client::ATTACK_OPTION::STEALMONSTER:
			if (In_eHitType == HIT_TYPE::STEALMONSTER)
			{
				_matrix vOtherWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
				vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, 0.f, 6.4f, 0.f));
				pOtherCharacter.lock()->Get_PhysX().lock()->Set_Position(
					vResultOtherWorldMatrix.r[3],
					GAMEINSTANCE->Get_DeltaTime(),
					Filters);
				pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_STEALCORVUS);
			}
			else if (In_eHitType == HIT_TYPE::LEFT_HIT)
			{
				Get_OwnerMonster()->Change_State<CBatBossState_HurtXL_F>();
			}
			else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
			{
				MONSTERTYPE eMonsterType = Get_OwnerMonster()->Get_MonsterType();
				pOtherCharacter.lock()->OnStealMonsterSkill(eMonsterType);
				Get_OwnerMonster()->Change_State<CBatBossState_HurtXL_L>();
			}
			break;
		}
		_float3 vShakingOffset = pOtherCharacter.lock()->Get_CurState().lock()->Get_ShakingOffset();
		_vector vShakingOffsetToVector = XMLoadFloat3(&vShakingOffset);
		_float fShakingRatio = 0.01f * iRand;



		//이거는한번만호출되게 해야함 
		//현재상태가 스턴스타트나 루프가아닌경우
		//혹시몰르니 예외처리해줌 
		if (Get_OwnerCharacter().lock()->Get_CurState().lock() != Get_Owner().lock()->Get_Component<CBatBossState_Stun_Start>().lock() &&
			Get_OwnerCharacter().lock()->Get_CurState().lock() != Get_Owner().lock()->Get_Component<CBatBossState_Stun_Loop>().lock())
		{
			if (pStatus.lock()->Is_Dead())
			{
				Get_OwnerCharacter().lock()->Change_State<CBatBossState_Stun_Start>(0.05f);
			}
		}
		else
		{
			//이떄 플레이어한테 이벤트를 던져줍시다
			if (pStatus.lock()->Get_Desc().m_iLifeCount == 2)
			{
				if (eAttackOption == ATTACK_OPTION::NONE ||
					eAttackOption == ATTACK_OPTION::NORMAL)
					pStatus.lock()->Minus_LifePoint(1);
					pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_BATEXECUTION);
					_matrix vOtherWorldMatrix = Get_OwnerCharacter().lock()->Get_Transform()->Get_WorldMatrix();
					vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(-1.2f, 0.f, 8.8f, 0.f));
					pOtherCharacter.lock()->Get_PhysX().lock()->Set_Position(
						vResultOtherWorldMatrix.r[3],
						GAMEINSTANCE->Get_DeltaTime(),
						Filters);
					pOtherCharacter.lock()->Get_Transform()->Set_Look2D(-vOtherWorldMatrix.r[2]);
					Get_Owner().lock()->Get_Component<CBatBossState_TakeExecution_Start>().lock()->Set_DieType(true);
					Get_OwnerCharacter().lock()->Change_State<CBatBossState_TakeExecution_Start>(0.05f);
				
			}
			else
			{
				if (eAttackOption == ATTACK_OPTION::NONE ||
					eAttackOption == ATTACK_OPTION::NORMAL)
					pOtherCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_BATEXECUTION);
					_matrix vOtherWorldMatrix = Get_OwnerCharacter().lock()->Get_Transform()->Get_WorldMatrix();
					vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(-1.2f, 0.f, 8.8f, 0.f));
					pOtherCharacter.lock()->Get_PhysX().lock()->Set_Position(
						vResultOtherWorldMatrix.r[3],
						GAMEINSTANCE->Get_DeltaTime(),
						Filters);
					pOtherCharacter.lock()->Get_Transform()->Set_Look2D(-vOtherWorldMatrix.r[2]);
					Get_Owner().lock()->Get_Component<CBatBossState_TakeExecution_Start>().lock()->Set_DieType(false);
					Get_OwnerCharacter().lock()->Change_State<CBatBossState_TakeExecution_Start>(0.05f);
				
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



void CBatBossStateBase::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);

}

void CBatBossStateBase::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);

}

void CBatBossStateBase::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);

}

void CBatBossStateBase::OnEventMessage(_uint iArg)
{
	
}

void CBatBossStateBase::Free()
{
}
