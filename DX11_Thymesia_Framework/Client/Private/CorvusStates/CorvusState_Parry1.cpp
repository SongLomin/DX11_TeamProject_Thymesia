#include "stdafx.h"
#include "CorvusStates/CorvusState_Parry1.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "Collider.h"
#include "Client_GameObjects.h"
#include "UI_DamageFont.h"

GAMECLASS_C(CCorvusState_Parry1);
CLONE_C(CCorvusState_Parry1, CComponent)

HRESULT CCorvusState_Parry1::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CCorvusState_Parry1::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_vShakingOffSet = { -1.f,0.2f,0.f };

	return S_OK;
}

void CCorvusState_Parry1::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD1_ParryL_NEW");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Parry1::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Parry1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	//Attack();
	Update_ParryType();

}

void CCorvusState_Parry1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_InputNextParry();

	Check_AndChangeNextState();
}

void CCorvusState_Parry1::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_Parry1::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{



#ifdef _DEBUG_COUT_
	// cout << "AttackIndex: " << m_iAttackIndex << endl;
#endif

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_Parry1::Update_ParryType()
{

	_uint		iKeyFrame = m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex();
	if (iKeyFrame >= 14 && iKeyFrame <= 25)
	{
		m_eParryType = PARRY_TYPE::PERFECT;
		return;
	}
	else if (iKeyFrame >= 1 && iKeyFrame <= 35)
	{
		m_eParryType = PARRY_TYPE::NORMAL;
		return;
	}
	else
	{
		m_eParryType = PARRY_TYPE::FAIL;
		return;
	}
}

void CCorvusState_Parry1::Attack()
{


}

void CCorvusState_Parry1::Check_InputNextParry()
{
	if (!KEY_INPUT(KEY::F, KEY_STATE::TAP))
	{
		return;
	}

	m_IsNextParry = true;

}

void CCorvusState_Parry1::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Weak_StaticCast<CCorvus>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(3.f, 3.f, 3.f));

	m_eParryType = PARRY_TYPE::PARRY_TYPE_END;

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Parry2>().lock())
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 13);
	}
	else
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	}

	if (!m_pModelCom.lock().get())
	{

		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_bParryed = false;



#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: Attack -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_Parry1::OnStateEnd()
{
	__super::OnStateEnd();

	//Disable_Weapons();
	m_IsNextParry = false;
	m_eParryType = PARRY_TYPE::PARRY_TYPE_END;
	m_bParryed = false;

	Weak_StaticCast<CCorvus>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));

}

void CCorvusState_Parry1::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CPlayerStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);

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
		weak_ptr<CStatus_Monster>	pMonsterStatusCom = Weak_StaticCast< CStatus_Monster>( pMonsterFromCharacter.lock()->Get_Status());

		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

		m_pTransformCom.lock()->LookAt2D(vSameHeightOtherColliderPosition);

		if (!pMonsterStatusCom.lock())
			MSG_BOX("Error : Can't Find CStatus_Monster From CorvusStateBase");

		PARRY_SUCCESS ParryType = (PARRY_SUCCESS)Check_AndChangeSuccessParrying(pMyCollider, pOtherCollider);

		_vector vViewPosition;
		_matrix ViewProjMatrix;

		vViewPosition = pMyCollider.lock()->Get_CurrentPosition();

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

		if (m_eParryType == PARRY_TYPE::PERFECT)
		{
			pDamageFont.lock()->SetUp_DamageFont(pStatus.lock()->Get_Desc().m_fParryingAtk * 2.f,
				vHitPos, ATTACK_OPTION::PARRY);
		}
		else
		{
			pDamageFont.lock()->SetUp_DamageFont(pStatus.lock()->Get_Desc().m_fParryingAtk,
				vHitPos, ATTACK_OPTION::PARRY);
		}
		_vector vShakingOffsetToVector;
		ATTACK_OPTION eAttackOption = pAttackArea.lock()->Get_OptionType();

		switch (eAttackOption)
		{
		case Client::ATTACK_OPTION::NOPARRYATTACK:
		{
			Check_AndChangeHitState(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
			pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);
		}
			break;
		default:
		{
			switch (m_eParryType)
			{

			case Client::PARRY_TYPE::PERFECT:
				//퍼펙트는 몬스터 게이지 많이깍고 플레이어피를채워\준다 상태는 왼쪽오른쪽 위아래 판단해서 상태를 그걸로바꿔주는용도
				switch (ParryType)
				{
				case Client::PARRY_SUCCESS::LEFT:
					pMonsterStatusCom.lock()->Add_ParryGauge(pStatus.lock()->Get_Desc().m_fParryingAtk * 2.f);

					GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
					pStatus.lock()->Heal_Player(30.f);
					Get_OwnerPlayer()->Change_State<CCorvusState_ParryDeflectLeft>();

					break;
				case Client::PARRY_SUCCESS::LEFTUP:
					pMonsterStatusCom.lock()->Add_ParryGauge(pStatus.lock()->Get_Desc().m_fParryingAtk * 2.f);
					GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
					pStatus.lock()->Heal_Player(30.f);
					Get_OwnerPlayer()->Change_State<CCorvusState_ParryDeflectLeftup>();
					break;
				case Client::PARRY_SUCCESS::RIGHT:
					pMonsterStatusCom.lock()->Add_ParryGauge(pStatus.lock()->Get_Desc().m_fParryingAtk * 2.f);
					GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
					pStatus.lock()->Heal_Player(30.f);
					Get_OwnerPlayer()->Change_State<CCorvusState_ParryDeflectRight>();
					break;
				case Client::PARRY_SUCCESS::RIGHTUP:
					pMonsterStatusCom.lock()->Add_ParryGauge(pStatus.lock()->Get_Desc().m_fParryingAtk * 2.f);
					GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
					pStatus.lock()->Heal_Player(30.f);
					break;
				case Client::PARRY_SUCCESS::FAIL:
					Check_AndChangeHitState(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
					pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);
					break;
				}

				vShakingOffsetToVector = XMLoadFloat3(&m_vShakingOffSet);

				GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffsetToVector, 0.2f, 0.7f, 9.f, 0.5f);
				GAMEINSTANCE->Set_MotionBlur(0.05f);

				break;
			case Client::PARRY_TYPE::NORMAL:
				//퍼펙트는 몬스터 게이지 적게깍고 플레이어피는안달고  상태는 왼쪽오른쪽 위아래 판단해서 상태를 그걸로바꿔주는용도
				switch (ParryType)
				{
				case Client::PARRY_SUCCESS::LEFT:
					pMonsterStatusCom.lock()->Add_ParryGauge(pStatus.lock()->Get_Desc().m_fParryingAtk);
					GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
					Get_OwnerPlayer()->Change_State<CCorvusState_ParryDeflectLeft>();
					break;
				case Client::PARRY_SUCCESS::LEFTUP:
					pMonsterStatusCom.lock()->Add_ParryGauge(pStatus.lock()->Get_Desc().m_fParryingAtk);
					GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
					Get_OwnerPlayer()->Change_State<CCorvusState_ParryDeflectLeftup>();
					break;
				case Client::PARRY_SUCCESS::RIGHT:
					pMonsterStatusCom.lock()->Add_ParryGauge(pStatus.lock()->Get_Desc().m_fParryingAtk);
					GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
					Get_OwnerPlayer()->Change_State<CCorvusState_ParryDeflectRight>();
					break;
				case Client::PARRY_SUCCESS::RIGHTUP:
					pMonsterStatusCom.lock()->Add_ParryGauge(pStatus.lock()->Get_Desc().m_fParryingAtk);
					GET_SINGLE(CGameManager)->Use_EffectGroup("BasicHitParticle", m_pTransformCom, (_uint)TIMESCALE_LAYER::MONSTER);
					Get_OwnerPlayer()->Change_State<CCorvusState_ParryDeflectRightup>();
					break;
				case Client::PARRY_SUCCESS::FAIL:
					Check_AndChangeHitState(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
					pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);
					break;
				}

				vShakingOffsetToVector = XMLoadFloat3(&m_vShakingOffSet);
				GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffsetToVector, 0.15f, 0.7f, 9.f, 0.5f);//일반 공격
				GAMEINSTANCE->Set_MotionBlur(0.05f);

				break;
			case Client::PARRY_TYPE::FAIL:
				Check_AndChangeHitState(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
				pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);
				break;
			}

			_bool bGroggy = pMonsterStatusCom.lock()->Is_Groggy();

			if (bGroggy)
			{
				pMonsterFromCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_GROGGY);
			}
		}
			break;
		}
		

	}
}


void CCorvusState_Parry1::OnEventMessage(_uint iArg)
{
}

void CCorvusState_Parry1::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Parry1::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Parry1::Free()
{

}

_bool CCorvusState_Parry1::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;



	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 35)
	{
		if (Check_RequirementAttackState())
		{
			weak_ptr<CGameObject> pTargetObject;

			if (Check_RequirementExcuteState(pTargetObject))
			{
				_vector vTargetPos = pTargetObject.lock()->Get_Transform()->Get_Position();
				m_pTransformCom.lock()->LookAt2D(vTargetPos);
				Get_OwnerPlayer()->Change_State<CCorvusState_NorMob_Execution>();
				Get_OwnerPlayer()->Get_CurState().lock()->OnEventMessage(Weak_Cast<CBase>(pTargetObject));
			}
			else
			{
				Rotation_InputToLookDir();

				Get_OwnerPlayer()->Change_State<CCorvusState_LAttack1>();

			}

			return true;
		}

		if (Check_RequirementAVoidState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_AVoid>();
			return true;
		}

		if (Check_RequirementClawAttackState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackTab>();
			return true;
		}
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 70)
	{
		if ((Check_RequirementRunState()))
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
			return true;
		}
	}


	if (Check_RequirementNextParryState())
	{
		if (Check_RequirementParryState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Parry2>();
			return true;
		}
	}


	if (Check_RuquireMnetFirstParryState())
	{
		if (Check_RequirementParryState())
		{
			if (!Rotation_InputToLookDir())
				Rotation_TargetToLookDir();

			Get_OwnerPlayer()->Change_State<CCorvusState_Parry1>();
			return false;
		}
	}







	return false;
}

_bool CCorvusState_Parry1::Check_RequirementNextParryState()
{
	_uint iTargetKeyFramefirst = 17;
	_uint iTargetKeyFrameSecond = 50;

	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFramefirst, iTargetKeyFrameSecond) && m_IsNextParry)
	{
		return true;
	}

	return false;
}

_bool CCorvusState_Parry1::Check_RuquireMnetFirstParryState()
{
	_uint iTargetKeyFrameMin = 51;
	_uint iTargetKeyFrameMax = 110;

	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) && m_IsNextParry)
	{
		return true;
	}

	return false;
}

