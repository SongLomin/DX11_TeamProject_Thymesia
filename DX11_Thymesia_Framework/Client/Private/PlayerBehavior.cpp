#include "stdafx.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Client_Components.h"
#include "PlayerBehavior.h"
#include "GameManager.h"
#include "HyperSpace.h"
#include "SMath.h"
#include "Player.h"
#include "Monster.h"

GAMECLASS_C(CPlayerBehavior);

CPlayerBehavior::CPlayerBehavior(const CPlayerBehavior& rhs)
{
	*this = rhs;
}

void CPlayerBehavior::Start()
{
	if (m_bLobby)
	{
		Change_State(m_pOwner.lock()->Get_Component<CLuxiyaState_LobbyStand>());
		m_pOwner.lock()->Get_Component<CSkillChain>().lock()->Set_Enable(false);
		m_pOwner.lock()->Get_Component<CCollider>().lock()->Set_Enable(false);
	}
	else
	{
		Change_State(m_pOwner.lock()->Get_Component<CLuxiyaState_Stand>());
	}
	m_pTransformCom = m_pOwner.lock()->Get_Component<CTransform>();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_pOwner.lock()->Get_Component<CModel>().lock()->CallBack_AnimationEnd += bind(&CPlayerBehavior::Call_AnimationEnd, this);
	m_vLookAtDir = _float3(0.f, 0.f, 0.f);
	m_pOwnerFromPlayer = Weak_Cast<CPlayer>(m_pOwner);
}

void CPlayerBehavior::Call_AnimationEnd()
{
	m_bAnimationEnd = true;
}

void CPlayerBehavior::Set_LobbyState()
{
	m_bLobby = true;
}

void CPlayerBehavior::Turn_Transform(_float fTimeDelta)
{
	if (m_bEdit)
		return;

	_matrix CurrentCamWorldMatrix;

	CurrentCamWorldMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD);
	CurrentCamWorldMatrix = SMath::Get_MatrixNormalize(CurrentCamWorldMatrix);

	_vector vLookDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	if (KEY_INPUT(KEY::W, KEY_STATE::HOLD))
	{
		vLookDir += CurrentCamWorldMatrix.r[2];
	}

	if (KEY_INPUT(KEY::A, KEY_STATE::HOLD))
	{
		vLookDir += -CurrentCamWorldMatrix.r[0];
	}

	if (KEY_INPUT(KEY::S, KEY_STATE::HOLD))
	{
		vLookDir += -CurrentCamWorldMatrix.r[2];
	}

	if (KEY_INPUT(KEY::D, KEY_STATE::HOLD))
	{
		vLookDir += CurrentCamWorldMatrix.r[0];
	}

	//새로운 회전 값이 있을 경우
	if (XMVector3Length(vLookDir).m128_f32[0] > DBL_EPSILON)
	{
		vLookDir.m128_f32[1] = 0.f;
		vLookDir = XMVector3Normalize(vLookDir);
		XMStoreFloat3(&m_vLookAtDir, vLookDir);
		m_fCurrentRotateTime = 0.f;
	}

	//갱신되거나 기존 회전 값이 있는 경우
	if (XMVector3Length(XMLoadFloat3(&m_vLookAtDir)).m128_f32[0] > DBL_EPSILON)
	{
		m_fCurrentRotateTime += fTimeDelta;
		_vector PlayerLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
		PlayerLook = XMVector3Normalize(PlayerLook);

		//m_vLookAtDir 
		_vector LookAtCross = XMVectorSet(-m_vLookAtDir.z, 0.f, m_vLookAtDir.x, 0.f);
		const _vector LookAtDir = XMLoadFloat3(&m_vLookAtDir);


		if (XMVector3Dot(LookAtDir, PlayerLook).m128_f32[0] > 0.95f)
		{
			_vector LookPos = m_pTransformCom.lock()->Get_State(CTransform::STATE::STATE_TRANSLATION) + LookAtDir;
			m_pTransformCom.lock()->LookAt(LookPos);

			m_vLookAtDir = _float3(0.f, 0.f, 0.f);

			return;
		}

		_float fDirResult = XMVector3Dot(LookAtCross, PlayerLook).m128_f32[0];

		if (fDirResult > 0.f)
		{
			m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), sqrtf(m_fCurrentRotateTime));
		}

		else
		{
			m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -sqrtf(m_fCurrentRotateTime));
		}
	}
}

_vector CPlayerBehavior::Get_InputToLookDir()
{
	if (m_bEdit)
		return XMVectorSet(0.f, 0.f, 0.f, 0.f);

	_matrix CurrentCamWorldMatrix;

	CurrentCamWorldMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD);
	CurrentCamWorldMatrix = SMath::Get_MatrixNormalize(CurrentCamWorldMatrix);

	_vector vLookDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	if (KEY_INPUT(KEY::W, KEY_STATE::HOLD))
	{
		vLookDir += CurrentCamWorldMatrix.r[2];
	}

	if (KEY_INPUT(KEY::A, KEY_STATE::HOLD))
	{
		vLookDir += -CurrentCamWorldMatrix.r[0];
	}

	if (KEY_INPUT(KEY::S, KEY_STATE::HOLD))
	{
		vLookDir += -CurrentCamWorldMatrix.r[2];
	}

	if (KEY_INPUT(KEY::D, KEY_STATE::HOLD))
	{
		vLookDir += CurrentCamWorldMatrix.r[0];
	}

	return vLookDir;
}

_bool CPlayerBehavior::Rotation_InputToLookDir()
{
	_vector vInputDir = Get_InputToLookDir();

	if (XMVector3Length(vInputDir).m128_f32[0] > DBL_EPSILON)
	{
		vInputDir.m128_f32[1] = 0.f;
		vInputDir = XMVector3Normalize(vInputDir);

		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
		m_pTransformCom.lock()->LookAt(vMyPosition + vInputDir);

		return true;
	}

	return false;
}

_bool CPlayerBehavior::Rotation_TargetToLookDir()
{
	weak_ptr<CMonster> pMonster = m_pOwnerFromPlayer.lock()->Get_TargetMonster();

	if (!pMonster.lock().get())
		return false;

	_vector MonsterPosition = pMonster.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);

	m_pTransformCom.lock()->LookAt(MonsterPosition);

	return true;
}

void CPlayerBehavior::Open_HyperSpace()
{
	weak_ptr<CGameObject> HyperSpaceGameObject = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::HYPERSPACE).front();
	weak_ptr<CHyperSpace> HyperSpace = Weak_Cast<CHyperSpace>(HyperSpaceGameObject);

	//하이퍼스페이스 객체가 없음. 혹은 하이퍼 스페이스 객체가 아닌 다른 객체가 담겨있음.
	if (!HyperSpace.lock().get())
		DEBUG_ASSERT;

	if (HyperSpace.lock()->Get_Enable())
		return;

	HyperSpace.lock()->Open_HyperSpace(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CPlayerBehavior::OnDestroy()
{
	m_pOwner.lock()->Get_Component<CModel>().lock()->CallBack_AnimationEnd -= bind(&CPlayerBehavior::Call_AnimationEnd, this);
}

void CPlayerBehavior::Free()
{
}