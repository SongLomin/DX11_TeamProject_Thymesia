#include "stdafx.h"
#include "AIStateBase.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameManager.h"
#include "Monster.h"
#include "Model.h"
#include "Player.h"
#include "NorMonStateBase.h"

GAMECLASS_C(CAIStateBase);

HRESULT CAIStateBase::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAIStateBase::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	CMonster::STATE_LINK_DESC StateLinkDesc;
	ZeroMemory(&StateLinkDesc, sizeof(CMonster::STATE_LINK_DESC));
	memcpy(&StateLinkDesc, pArg, sizeof(CMonster::STATE_LINK_DESC));

	m_eNorMonType = StateLinkDesc.eNorMonType;
	m_eNorMonIdleType = StateLinkDesc.eNorMonIdleType;
	m_fStartPosition = StateLinkDesc.m_fStartPositon;
	

	m_iTimeScaleLayer = (_uint)TIMESCALE_LAYER::MONSTER;
	m_pOwnerFromMonster = Weak_Cast<CMonster>(m_pOwner);
	
	return S_OK;
}

void CAIStateBase::Start()
{

	__super::Start();

	m_pTransformCom = Get_Owner().lock()->Get_Component<CTransform>();
}

void CAIStateBase::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CAIStateBase::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CAIStateBase::Check_RequirementCoolDown(weak_ptr<CAIStateBase> pTargetState, const _float& In_fCoolTime)
{
	_float fCheckStateTimeAcc = pTargetState.lock()->Get_StateTimeAcc();
	_float fOwnerTimeAcc = Get_OwnerMonster()->Get_TimeAcc();

	// ���� ��ü�� ���� �ð��� ���¿� ������� ���� ���� �ð� + ��Ÿ�� ��
	if (fOwnerTimeAcc > fCheckStateTimeAcc + In_fCoolTime)
	{
		return true;
	}
	// ��ü�� �޸� Ǯ�� ����ϱ� ������ ��ü�� ���� �ð��� ��Ȱ��ɶ����� 0���� �ʱ�ȭ������
	// ���´� �׷��� �ʴ�. ���� �� ���� -��� ���ο� ��ü�̹Ƿ� ��Ÿ���� �ٽ� ���.
	else if (fOwnerTimeAcc - fCheckStateTimeAcc < -DBL_EPSILON)
	{
		return true;
	}

	return false;
}




shared_ptr<CMonster> CAIStateBase::Get_OwnerMonster() const noexcept
{
	return m_pOwnerFromMonster.lock();
}

_bool CAIStateBase::Check_Requirement()
{
	

	return __super::Check_Requirement();
}

void CAIStateBase::Turn_ToThePlayer(_float fTimeDelta)
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock())
		return;

	_vector vCurPlayerPos = pCurrentPlayer.lock()->Get_WorldPosition();
	_vector vMyPos = Get_OwnerCharacter().lock()->Get_WorldPosition();
	_vector vLookAt = vCurPlayerPos - vMyPos;
	vLookAt.m128_f32[1] = 0.f;
	vLookAt = XMVector3Normalize(vLookAt);
	XMStoreFloat3(&m_vLookAtDir, vLookAt);


	//���ŵǰų� ���� ȸ�� ���� �ִ� ���
	if (XMVector3Length(XMLoadFloat3(&m_vLookAtDir)).m128_f32[0] > DBL_EPSILON)
	{
		m_fCurrentRotateTime += fTimeDelta * 0.3f;
		_vector AILook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
		AILook = XMVector3Normalize(AILook);

		//m_vLookAtDir 
		_vector LookAtCross = XMVectorSet(-m_vLookAtDir.z, 0.f, m_vLookAtDir.x, 0.f);
		const _vector LookAtDir = XMLoadFloat3(&m_vLookAtDir);


		if (XMVector3Dot(LookAtDir, AILook).m128_f32[0] > 0.99f)
		{
			_vector LookPos = m_pTransformCom.lock()->Get_State(CTransform::STATE::STATE_TRANSLATION) + LookAtDir;
			m_pTransformCom.lock()->LookAt(LookPos);

			m_vLookAtDir = _float3(0.f, 0.f, 0.f);
			m_fCurrentRotateTime = 0.f;

			return;
		}

		_float fDirResult = XMVector3Dot(LookAtCross, AILook).m128_f32[0];

		if (fDirResult > 0.f)
		{
			
			m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), sqrtf(fTimeDelta * 0.08f));
		}

		else
		{
			m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -sqrtf(fTimeDelta * 0.08f));
		}
	}
}

_vector CAIStateBase::Get_InputToLookDir()
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

void CAIStateBase::StartPositonLookAt(_float fTimeDelta)
{
	_vector vStartPosition = XMLoadFloat4(&m_fStartPosition);

	m_pTransformCom.lock()->LookAt2D(vStartPosition);
}

_bool CAIStateBase::Rotation_InputToLookDir()
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

_bool CAIStateBase::Rotation_TargetToLookDir()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock().get())
		return false;

	_vector CharacterPosition = pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);

	m_pTransformCom.lock()->LookAt2D(CharacterPosition);

	return true;
}

_float CAIStateBase::Get_DistanceWithPlayer() const
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	_vector vPlayerPosition = pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVector3Length(vPlayerPosition - vMyPosition).m128_f32[0];

	return fDistance;
}



_vector CAIStateBase::Get_CurMonToStartMonDir()
{
	_vector vCurrenPosition = m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vStartPosition = XMLoadFloat4(&m_fStartPosition);
	_vector vLookDir = XMVector4Normalize(vStartPosition - vCurrenPosition);

	return vLookDir;
}

_float CAIStateBase::GetStartPositionToCurrentPositionDir()
{
	_vector vCurrenPosition = m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	vCurrenPosition = XMVectorSetY(vCurrenPosition, 0.f);
	_vector vStartPosition = XMLoadFloat4(&m_fStartPosition);
	vStartPosition = XMVectorSetY(vStartPosition, 0.f);
	_float fDistance = XMVectorGetX(XMVector3Length(vCurrenPosition - vStartPosition));

	return fDistance;
}



void CAIStateBase::TurnMechanism()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock())
		return;

	if (ComputeAngleWithPlayer() <= 0.f) // 90�϶� 0 90����ũ�� -���̴� 0 �����۴�
	{
		switch (ComputeDirectionToPlayer())
		{
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_TurnR90>(0.05f);
			break;
		case -1:
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_TurnL90>(0.05f);
			break;
		default:
			assert(0);
			return;
		}

		
	}

	else
	{
		//���õõ���ϱ�
		_vector vPlayerPosition = pCurrentPlayer.lock()->Get_WorldPosition();
		m_pTransformCom.lock()->LookAt2D(vPlayerPosition);
	}

}


_float CAIStateBase::ComputeAngleWithPlayer()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();


	_vector vCurPlayerPos = pCurrentPlayer.lock()->Get_WorldPosition();
	vCurPlayerPos.m128_f32[1] = 0.f;
	_vector vMyPos = Get_OwnerCharacter().lock()->Get_WorldPosition();
	vMyPos.m128_f32[1] = 0.f;
	_vector vMonsterToPlayerDirectionVector = XMVector3Normalize(vCurPlayerPos - vMyPos);
	_vector vMyLookVector = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	vMyLookVector.m128_f32[1] = 0.f;
	vMyLookVector = XMVector3Normalize(vMyLookVector);

	_float fCos = XMVectorGetY((XMVector3Dot(vMonsterToPlayerDirectionVector, vMyLookVector)));

	cout << "ComputeAngleWithPlayer: " << fCos << endl;

	return fCos;

}

_int CAIStateBase::ComputeDirectionToPlayer()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();


	_vector vCurPlayerPos = pCurrentPlayer.lock()->Get_WorldPosition();
	_vector vMyPos = Get_OwnerCharacter().lock()->Get_WorldPosition();
	_vector vMonsterToPlayerDirectionVector = XMVector3Normalize(vCurPlayerPos - vMyPos);
	_vector vMyLookVector = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);

	_float fCross = XMVectorGetY(XMVector3Cross(vMyLookVector, vMonsterToPlayerDirectionVector));

	if (fCross >= 0.f) // ��� ������
	{
		// �����ʰ����϶���Ʈ����<1����>
		return 1;
	}
	else // ���� ����
	{
		//�������ΰ����Ϸ���Ʈ���� <-1����>
		return -1;
	}
}



void CAIStateBase::OnDestroy()
{
	
	__super::OnDestroy();
}

void CAIStateBase::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);
}

void CAIStateBase::OnDisable()
{
	__super::OnDisable();
}

void CAIStateBase::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_fStateTimeAcc = Get_OwnerMonster()->Get_TimeAcc();
}

void CAIStateBase::OnStateEnd()
{
	__super::OnStateEnd();

	if (!m_pModelCom.lock())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}
}

void CAIStateBase::Free()
{
}
