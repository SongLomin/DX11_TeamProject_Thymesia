#include "stdafx.h"
#include "EliteMonState/EliteStateBase.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameManager.h"
#include "Monster.h"
#include "Model.h"
#include "Player.h"
#include "JokerStates.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"

GAMECLASS_C(CEliteStateBase);

HRESULT CEliteStateBase::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEliteStateBase::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_iTimeScaleLayer = (_uint)TIMESCALE_LAYER::MONSTER;
	m_pOwnerFromMonster = Weak_Cast<CMonster>(m_pOwner);
	Set_Enable(false);
	return S_OK;
}

void CEliteStateBase::Start()
{
	__super::Start();
	m_pTransformCom = Get_Owner().lock()->Get_Component<CTransform>();
}

void CEliteStateBase::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEliteStateBase::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CEliteStateBase::Check_RequirementCoolDown(weak_ptr<CEliteStateBase> pTargetState, const _float& In_fCoolTime)
{
	_float fCheckStateTimeAcc(pTargetState.lock()->Get_StateTimeAcc());
	_float fOwnerTimeAcc(Get_OwnerMonster()->Get_TimeAcc());

	if (fOwnerTimeAcc > fCheckStateTimeAcc + In_fCoolTime)
		return true;

	else if (fOwnerTimeAcc - fCheckStateTimeAcc < -DBL_EPSILON)
		return true;

	return false;
}

void CEliteStateBase::Init_Desc(void* In_pDesc)
{
	CMonster::STATE_LINK_MONSTER_DESC StateLinkDesc;
	ZeroMemory(&StateLinkDesc, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	memcpy(&StateLinkDesc, In_pDesc, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	m_eMonType        = StateLinkDesc.eMonType;
	m_eNorMonIdleType = StateLinkDesc.eNorMonIdleType;
	m_fStartPosition  = StateLinkDesc.m_fStartPositon;
	m_eBossStartType  = StateLinkDesc.eBossStartType;
}

void CEliteStateBase::Turn_ToThePlayer(_float fTimeDelta)
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock())
		return;

	_vector vCurPlayerPos(pCurrentPlayer.lock()->Get_WorldPosition());
	_vector vMyPos(Get_OwnerCharacter().lock()->Get_WorldPosition());
	_vector vLookAt(vCurPlayerPos - vMyPos);
	vLookAt = XMVector3Normalize(XMVectorSetY(vLookAt, 0.f));
	XMStoreFloat3(&m_vLookAtDir, vLookAt);

	if (XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vLookAtDir))) > DBL_EPSILON)
	{
		m_fCurrentRotateTime += fTimeDelta * 0.3f;
		_vector AILook(m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK));
		AILook = XMVector3Normalize(AILook);

		_vector LookAtCross(XMVectorSet(-m_vLookAtDir.z, 0.f, m_vLookAtDir.x, 0.f));
		const _vector LookAtDir(XMLoadFloat3(&m_vLookAtDir));

		if (0.99f < XMVectorGetX(XMVector3Dot(LookAtDir, AILook)))
		{
			_vector LookPos(m_pTransformCom.lock()->Get_State(CTransform::STATE::STATE_TRANSLATION) + LookAtDir);
			m_pTransformCom.lock()->LookAt(LookPos);

			m_vLookAtDir = _float3(0.f, 0.f, 0.f);
			m_fCurrentRotateTime = 0.f;
			return;
		}

		_float fDirResult(XMVectorGetX(XMVector3Dot(LookAtCross, AILook)));

		if (fDirResult > 0.f)
			m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), sqrtf(fTimeDelta * 0.08f));
		else
			m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -sqrtf(fTimeDelta * 0.08f));
	}
}

_vector CEliteStateBase::Get_InputToLookDir()
{
	_matrix CurrentCamWorldMatrix;
	CurrentCamWorldMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD);
	CurrentCamWorldMatrix = SMath::Get_MatrixNormalize(CurrentCamWorldMatrix);

	_vector vLookDir(XMVectorSet(0.f, 0.f, 0.f, 0.f));

	if (KEY_INPUT(KEY::W, KEY_STATE::HOLD))
		vLookDir += CurrentCamWorldMatrix.r[2];

	if (KEY_INPUT(KEY::A, KEY_STATE::HOLD))
		vLookDir += -CurrentCamWorldMatrix.r[0];

	if (KEY_INPUT(KEY::S, KEY_STATE::HOLD))
		vLookDir += -CurrentCamWorldMatrix.r[2];

	if (KEY_INPUT(KEY::D, KEY_STATE::HOLD))
		vLookDir += CurrentCamWorldMatrix.r[0];

	return vLookDir;
}

void CEliteStateBase::StartPositonLookAt(_float fTimeDelta)
{
	_vector vStartPosition(XMLoadFloat4(&m_fStartPosition));
	m_pTransformCom.lock()->LookAt2D(vStartPosition);
}

_bool CEliteStateBase::Rotation_InputToLookDir()
{
	_vector vInputDir(Get_InputToLookDir());

	if (XMVectorGetX(XMVector3Length(vInputDir)) > DBL_EPSILON)
	{
		vInputDir = XMVectorSetY(vInputDir, 0.f);
		vInputDir = XMVector3Normalize(vInputDir);

		_vector vMyPosition(m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION));
		m_pTransformCom.lock()->LookAt(vMyPosition + vInputDir);
		return true;
	}

	return false;
}

_bool CEliteStateBase::Rotation_TargetToLookDir()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock().get())
		return false;

	_vector CharacterPosition(pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION));
	m_pTransformCom.lock()->LookAt2D(CharacterPosition);
	return true;
}

_float CEliteStateBase::Get_DistanceWithPlayer() const
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	_vector vPlayerPosition(pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION));
	_vector vMyPosition(m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION));
	_float fDistance(XMVectorGetX(XMVector3Length(vPlayerPosition - vMyPosition)));
	return fDistance;
}

void CEliteStateBase::TurnMechanism()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock())
		return;

	_float fDistance(Get_DistanceWithPlayer());

	switch (m_eMonType)
	{

	case Client::MONSTERTYPE::JOKER:
	{
		if (3.f < fDistance)
		{
			if (ComputeAngleWithPlayer() <= 0.f) // 90일때 0 90보다크면 -값이다 90보다 작으면 +값이다
			{
				switch (ComputeDirectionToPlayer())
				{
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CJokerState_TurnR90>(0.05f);
					break;
				case -1:
					Get_OwnerCharacter().lock()->Change_State<CJokerState_TurnL90>(0.05f);
					break;
				default:
					assert(0);
					return;
				}
			}
			else
			{
				Rotation_TargetToLookDir();
				Get_Owner().lock()->Get_Component<CJokerState_Idle>().lock()->Set_TurnCheck(false);
				Get_OwnerCharacter().lock()->Change_State<CJokerState_Idle>(0.05f);
			}
		}
		else
		{
			if (ComputeAngleWithPlayer() <= 0.f)
			{
				if (ComputeDirectionToPlayer() == 1)
					Get_OwnerCharacter().lock()->Change_State<CJokerState_TurnAtkR>(0.05f);
				else
					Get_OwnerCharacter().lock()->Change_State<CJokerState_TurnAtkL>(0.05f);
			}
		}
	}
		break;
	case Client::MONSTERTYPE::BIGHANDMAN:
	{

		if (ComputeAngleWithPlayer() <= 0.f) // 90일때 0 90보다크면 -값이다 0 보다작다
		{
			switch (ComputeDirectionToPlayer())
			{
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CBigHandManState_TurnR90>(0.05f);
				break;
			case -1:
				Get_OwnerCharacter().lock()->Change_State<CBigHandManState_TurnL90>(0.05f);
				break;
			default:
				assert(0);
				return;
			}


		}

		else
		{
			switch (ComputeDirectionToPlayer())
			{
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CBigHandManState_TurnR90>(0.05f);
				break;
			case -1:
				Get_OwnerCharacter().lock()->Change_State<CBigHandManState_TurnL90>(0.05f);
				break;
			default:
				assert(0);
				return;
			}
		}
	}
		break;

	}

	
}

void CEliteStateBase::TurnAttack(_float fTimeDelta)
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	_vector vCurPlayerPos(pCurrentPlayer.lock()->Get_WorldPosition());
	_vector vMyPos(Get_OwnerCharacter().lock()->Get_WorldPosition());
	_vector vMonsterToPlayerDirectionVector(XMVector3Normalize(vCurPlayerPos - vMyPos));
	_vector vMyLookVector(m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK));
	_float fCross(XMVectorGetY(XMVector3Cross(vMyLookVector, vMonsterToPlayerDirectionVector)));
	if (fCross >= 0.f)
		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), sqrtf(fTimeDelta * 0.08f));
	else
		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -sqrtf(fTimeDelta * 0.08f));
}

void CEliteStateBase::OnDestroy()
{
	__super::OnDestroy();
}

void CEliteStateBase::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);
}

void CEliteStateBase::OnDisable()
{
	__super::OnDisable();
}

void CEliteStateBase::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock())
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();

	m_fStateTimeAcc = Get_OwnerMonster()->Get_TimeAcc();
}

void CEliteStateBase::OnStateEnd()
{
	__super::OnStateEnd();

	if (!m_pModelCom.lock())
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
}

void CEliteStateBase::Free()
{
}
