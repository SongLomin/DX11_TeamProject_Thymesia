#include "stdafx.h"
#include "PlayerStateBase.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameManager.h"
#include "Monster.h"
#include "Player.h"
#include "Model.h"

GAMECLASS_C(CPlayerStateBase);

HRESULT CPlayerStateBase::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerStateBase::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iTimeScaleLayer = (_uint)TIMESCALE_LAYER::PLAYER;
	m_pOwnerFromPlayer = Weak_Cast<CPlayer>(m_pOwner);
	m_vLookAtDir = { 0.f, 0.f, 0.f };
	return S_OK;
}

void CPlayerStateBase::Start()
{
	__super::Start();
}

void CPlayerStateBase::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CPlayerStateBase::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

shared_ptr<CPlayer> CPlayerStateBase::Get_OwnerPlayer() const noexcept
{
	return m_pOwnerFromPlayer.lock();
}

_bool CPlayerStateBase::Check_Requirement()
{
	if (!__super::Check_Requirement())
	{
		return false;
	}

	return true;
}

void CPlayerStateBase::Turn_Transform(_float fTimeDelta)
{
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

_vector CPlayerStateBase::Get_InputToLookDir()
{
	/*if (m_bEdit)
		return XMVectorSet(0.f, 0.f, 0.f, 0.f);*/

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

_bool CPlayerStateBase::Rotation_InputToLookDir()
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

_bool CPlayerStateBase::Rotation_TargetToLookDir()
{
	weak_ptr<CMonster> pMonster = m_pOwnerFromPlayer.lock()->Get_TargetMonster();

	if (!pMonster.lock().get())
		return false;

	_vector MonsterPosition = pMonster.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);

	m_pTransformCom.lock()->LookAt(MonsterPosition);

	return true;
}

_bool CPlayerStateBase::Rotation_NearToLookDir()
{
	list<weak_ptr<CGameObject>> pMonsters = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::MONSTER);

	if (pMonsters.empty())
	{
		return false;
	}

	_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMonsterPosition;
	weak_ptr<CGameObject> pTargetMonster = pMonsters.front();
	_float fMinDistance = 99999.f;
	_float fDistance = 0.f;

	for (auto& elem : pMonsters)
	{
		vMonsterPosition = elem.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
		fDistance = XMVector3Length(vMonsterPosition - vMyPosition).m128_f32[0];

		if (fDistance < fMinDistance)
		{
			pTargetMonster = elem;
			fMinDistance = fDistance;
		}
	}

	m_pTransformCom.lock()->LookAt2D(pTargetMonster.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION));

	return true;
}


void CPlayerStateBase::OnWeaponFirstAttack(weak_ptr<CCollider> pOtherCollider)
{
	//GET_SINGLE(CGameManager)->Use_EffectGroup("ShockWave", m_pTransformCom);

}

void CPlayerStateBase::OnWeaponAttack(weak_ptr<CCollider> pOtherCollider)
{
}

void CPlayerStateBase::OnDestroy()
{
	
	__super::OnDestroy();
}

void CPlayerStateBase::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);
}

void CPlayerStateBase::OnDisable()
{
	__super::OnDisable();
}

void CPlayerStateBase::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}
}

void CPlayerStateBase::OnStateEnd()
{
	__super::OnStateEnd();

	if (!m_pModelCom.lock())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}
}

void CPlayerStateBase::Free()
{
}
