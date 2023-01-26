#include "stdafx.h"
#include "BossStateBase.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameManager.h"
#include "Monster.h"
#include "Model.h"
#include "Player.h"
#include  "VargStates.h"
#include "BossBat/BatStates.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CBossStateBase);

HRESULT CBossStateBase::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossStateBase::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iTimeScaleLayer = (_uint)TIMESCALE_LAYER::MONSTER;
	m_pOwnerFromMonster = Weak_Cast<CMonster>(m_pOwner);

	return S_OK;
}

void CBossStateBase::Start()
{

	__super::Start();

	m_pTransformCom = Get_Owner().lock()->Get_Component<CTransform>();
}

void CBossStateBase::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CBossStateBase::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CBossStateBase::Check_RequirementCoolDown(weak_ptr<CBossStateBase> pTargetState, const _float& In_fCoolTime)
{
	_float fCheckStateTimeAcc = pTargetState.lock()->Get_StateTimeAcc();
	_float fOwnerTimeAcc = Get_OwnerMonster()->Get_TimeAcc();

	// 현재 객체의 누적 시간과 상태에 적어놓은 상태 진입 시간 + 쿨타임 비교
	if (fOwnerTimeAcc > fCheckStateTimeAcc + In_fCoolTime)
	{
		return true;
	}
	// 객체는 메모리 풀을 사용하기 때문에 객체의 누적 시간은 재활용될때마다 0으로 초기화되지만
	// 상태는 그렇지 않다. 따라서 뺀 값이 -라면 새로운 객체이므로 쿨타임을 다시 잰다.
	else if (fOwnerTimeAcc - fCheckStateTimeAcc < -DBL_EPSILON)
	{
		return true;
	}

	return false;
}

void CBossStateBase::Init_Desc(void* In_pDesc) 
{
	CMonster::STATE_LINK_MONSTER_DESC StateLinkDesc;
	ZeroMemory(&StateLinkDesc, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	memcpy(&StateLinkDesc, In_pDesc, sizeof(CMonster::STATE_LINK_MONSTER_DESC));

	m_eMonType        = StateLinkDesc.eMonType;
	m_eNorMonIdleType = StateLinkDesc.eNorMonIdleType;
	m_fStartPosition  = StateLinkDesc.m_fStartPositon;
	m_eBossStartType  = StateLinkDesc.eBossStartType;
}


shared_ptr<CMonster> CBossStateBase::Get_OwnerMonster() const noexcept
{
	return m_pOwnerFromMonster.lock();
}

_bool CBossStateBase::Check_Requirement()
{
	return __super::Check_Requirement();
}

void CBossStateBase::Turn_ToThePlayer(_float fTimeDelta)
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


	//갱신되거나 기존 회전 값이 있는 경우
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

_vector CBossStateBase::Get_InputToLookDir()
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

void CBossStateBase::StartPositonLookAt(_float fTimeDelta)
{
	_vector vStartPosition = XMLoadFloat4(&m_fStartPosition);

	m_pTransformCom.lock()->LookAt2D(vStartPosition);
}

_bool CBossStateBase::Rotation_InputToLookDir()
{
	_vector vInputDir = Get_InputToLookDir();

	if (XMVector3Length(vInputDir).m128_f32[0] > DBL_EPSILON)
	{
		vInputDir.m128_f32[1] = 0.f;
		vInputDir = XMVector3Normalize(vInputDir);

		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
		m_pTransformCom.lock()->LookAt2D(vMyPosition + vInputDir);

		return true;
	}

	return false;
}	

_bool CBossStateBase::Rotation_TargetToLookDir()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock().get())
		return false;

	_vector CharacterPosition = pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);

	if (!m_pTransformCom.lock())
		return false;

	m_pTransformCom.lock()->LookAt2D(CharacterPosition);

	return true;
}

_bool CBossStateBase::Rotation_TargetToLookRevirseDir()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock().get())
		return false;

	_vector CharacterPosition = pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);

	m_pTransformCom.lock()->LookAt2D(-CharacterPosition);
	
	return true;
}

_bool CBossStateBase::JumpLookOffsetLookAt()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock().get())
		return false;

	_vector CharacterPosition = pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);

	if (ComputeAngleWithPlayer() >= 0.f)  //양수면 등지고있다 
	{
		
	      _vector vOtherPos = 	pCurrentPlayer.lock()->Get_WorldPosition();
		  _vector vOtherLook = pCurrentPlayer.lock()->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		  
		  m_pTransformCom.lock()->LookAt2D(vOtherPos + vOtherLook * -8.f);

		


	}
	else //음수면 바라보고있다
	{
		_vector vOtherPos = pCurrentPlayer.lock()->Get_WorldPosition();
		_vector vOtherLook = pCurrentPlayer.lock()->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		m_pTransformCom.lock()->LookAt2D(vOtherPos + vOtherLook * 8.f);

	}

	return true;
}


_float CBossStateBase::Get_DistanceWithPlayer() const
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	_vector vPlayerPosition = pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVector3Length(vPlayerPosition - vMyPosition).m128_f32[0];

	return fDistance;
}




_vector CBossStateBase::Get_CurMonToStartMonDir()
{
	_vector vCurrenPosition = m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	vCurrenPosition = XMVectorSetY(vCurrenPosition, 0.f);
	_vector vStartPosition = XMLoadFloat4(&m_fStartPosition);
	vStartPosition = XMVectorSetY(vStartPosition, 0.f);
	_vector vLookDir = XMVector4Normalize(vStartPosition - vCurrenPosition);

	return vLookDir;
}

_float CBossStateBase::GetStartPositionToCurrentPositionDir()
{
	_vector vCurrenPosition = m_pOwner.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	vCurrenPosition = XMVectorSetY(vCurrenPosition, 0.f);
	_vector vStartPosition = XMLoadFloat4(&m_fStartPosition);
	vStartPosition = XMVectorSetY(vStartPosition, 0.f);
	_float fDistance = XMVectorGetX(XMVector3Length(vCurrenPosition - vStartPosition));

	return fDistance;
}



void CBossStateBase::TurnMechanism()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();


	if (!pCurrentPlayer.lock())
		return;

	if (m_eMonType == MONSTERTYPE::TYPE_END)
		return;

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::VARG:
	{
		_float fDistance = Get_DistanceWithPlayer();


		if (fDistance > 3.f)
		{
			if (ComputeAngleWithPlayer() <= 0.f) // 90일때 0 90보다크면 -값이다 90보다 작으면 +값이다
			{
				switch (ComputeDirectionToPlayer())
				{
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CVargBossState_TurnR>(0.05f);
					break;
				case -1:
					Get_OwnerCharacter().lock()->Change_State<CVargBossState_TurnL>(0.05f);
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
					Get_OwnerCharacter().lock()->Change_State<CVargBossState_TurnR>(0.05f);
					break;
				case -1:
					Get_OwnerCharacter().lock()->Change_State<CVargBossState_TurnL>(0.05f);
					break;
				default:
					assert(0);
					return;
				}
			}
		}
		else
		{
			if (ComputeAngleWithPlayer() <= 0.f)
			{
				Get_OwnerCharacter().lock()->Change_State<CVargBossState_TurnAttack>(0.05f);
			}
			else
			{
				Rotation_TargetToLookDir();
				Get_Owner().lock()->Get_Component<CVargBossState_Idle>().lock()->Set_TurnCheck(false);
				Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
			}

		}
	}
		

		break;
	case Client::MONSTERTYPE::URD:
	{
		_float fDistance = Get_DistanceWithPlayer();
		if (ComputeAngleWithPlayer() <= 0.f) // 90일때 0 90보다크면 -값이다 90보다 작으면 +값이다
		{
			switch (ComputeDirectionToPlayer())
			{
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_TurnR>(0.05f);
				break;
			case -1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_TurnL>(0.05f);
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
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_TurnR>(0.05f);
				break;
			case -1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_TurnL>(0.05f);
				break;
			default:
				assert(0);
				return;
			}
		}

	}
		
		break;
	case Client::MONSTERTYPE::BAT:
	{
		_float fDistance = Get_DistanceWithPlayer();


		if (fDistance > 6.f)
		{
			if (ComputeAngleWithPlayer() <= 0.f) // 90일때 0 90보다크면 -값이다 90보다 작으면 +값이다
			{
				switch (ComputeDirectionToPlayer())
				{
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CBatBossState_TurnR>(0.05f);
					break;
				case -1:
					Get_OwnerCharacter().lock()->Change_State<CBatBossState_TurnL>(0.05f);
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
					Get_OwnerCharacter().lock()->Change_State<CBatBossState_TurnR>(0.05f);
					break;
				case -1:
					Get_OwnerCharacter().lock()->Change_State<CBatBossState_TurnL>(0.05f);
					break;
				default:
					assert(0);
					return;
				}
			}
		}
		else
		{
			//오른쪽 왼쪾 구분해주고 	
			if (ComputeAngleWithPlayer() <= 0.f)
			{
				switch (ComputeDirectionToPlayer())
				{
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Atk_R01_2a>(0.05f);
					break;
				case -1:
					Get_OwnerCharacter().lock()->Change_State<CBatBossState_Atk_L01_2a>(0.05f);
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
					Get_OwnerCharacter().lock()->Change_State<CBatBossState_TurnR>(0.05f);
					break;
				case -1:
					Get_OwnerCharacter().lock()->Change_State<CBatBossState_TurnL>(0.05f);
					break;
				default:
					assert(0);
					return;
				}
			}

		}
	}
		break;
	}

}

void CBossStateBase::TurnAttack(_float fTimeDelta)
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();


	_vector vCurPlayerPos = pCurrentPlayer.lock()->Get_WorldPosition();
	_vector vMyPos = Get_OwnerCharacter().lock()->Get_WorldPosition();
	_vector vMonsterToPlayerDirectionVector = XMVector3Normalize(vCurPlayerPos - vMyPos);
	_vector vMyLookVector = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);

	_float fCross = XMVectorGetY(XMVector3Cross(vMyLookVector, vMonsterToPlayerDirectionVector));

	if (fCross >= 0.f) // 양수 오른쪽
	{
		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), sqrtf(fTimeDelta * 0.08f));
	}
	else // 음수 왼쪽
	{
		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -sqrtf(fTimeDelta * 0.08f));
	}
}


_float CBossStateBase::ComputeAngleWithPlayer()
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

#ifdef _DEBUG_COUT_
	cout << "ComputeAngleWithPlayer: " << fCos << endl;
#endif

	return fCos;

}

_int CBossStateBase::ComputeDirectionToPlayer()
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();


	_vector vCurPlayerPos = pCurrentPlayer.lock()->Get_WorldPosition();
	_vector vMyPos = Get_OwnerCharacter().lock()->Get_WorldPosition();
	_vector vMonsterToPlayerDirectionVector = XMVector3Normalize(vCurPlayerPos - vMyPos);
	_vector vMyLookVector = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);

	_float fCross = XMVectorGetY(XMVector3Cross(vMyLookVector, vMonsterToPlayerDirectionVector));

	if (fCross >= 0.f) // 양수 오른쪽
	{
		// 오른쪽가는턴라이트싱행<1리턴>
		return 1;
	}
	else // 음수 왼쪽
	{
		//왼쪽으로가는턴래프트실행 <-1리턴>
		return -1;
	}
}



void CBossStateBase::OnDestroy()
{

	__super::OnDestroy();
}

void CBossStateBase::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);
}

void CBossStateBase::OnDisable()
{
	__super::OnDisable();
}

void CBossStateBase::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_fStateTimeAcc = Get_OwnerMonster()->Get_TimeAcc();
}

void CBossStateBase::OnStateEnd()
{
	__super::OnStateEnd();

	if (!m_pModelCom.lock())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}
}

void CBossStateBase::Free()
{
}
