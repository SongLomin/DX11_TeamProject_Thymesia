#include "stdafx.h"
#include "Camera_Target.h"
#include "GameManager.h"
#include "Monster.h"
#include "Player.h"
#include "GameObject.h"
#include "Model.h"
#include "BoneNode.h"

GAMECLASS_C(CCamera_Target);
CLONE_C(CCamera_Target, CGameObject);

CCamera_Target::CCamera_Target(const CCamera_Target& rhs)
{
	*this = rhs;
}

HRESULT CCamera_Target::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CCamera_Target::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pTransformCom.lock()->Add_Position(XMVectorSet(0.f, 2.f, 0.f, 1.f));
	XMStoreFloat4x4(&m_CinemaWorldMatrix, XMMatrixIdentity());

	//_matrix MatLookAtZeroPoint = XMMatrixIdentity();
	//MatLookAtZeroPoint.r[3] = XMVectorSet(0.f, 3.5f, -3.5f, 1.f);
	//MatLookAtZeroPoint = SMath::LookAt(MatLookAtZeroPoint, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	//_vector vLookAtZeroPointQuat = XMQuaternionRotationMatrix(SMath::Get_RotationMatrix(MatLookAtZeroPoint));

	GET_SINGLE(CGameManager)->Use_EffectGroup("Tutorial_Dust", m_pTransformCom);

	USE_START(CCamera_Target);

	return S_OK;
}

HRESULT CCamera_Target::Start()
{
	__super::Start();

	m_pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	//현재 플레이어가 NULLPTR임.
	if (!m_pCurrentPlayer.lock().get())
		DEBUG_ASSERT;

	m_pCurrentPlayerTransformCom = m_pCurrentPlayer.lock()->Get_Component<CTransform>();
	XMStoreFloat4(&m_vPlayerFollowLerpPosition, m_pCurrentPlayerTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION));

	return S_OK;
}

void CCamera_Target::Tick(_float fTimeDelta)
{
	if (GetFocus() != g_hWnd)
	{
		return;
	}

	if (!m_pCurrentPlayer.lock().get())
		return;

	if(m_bCinematic)
	{ 
		Update_Bone();
	}

	else
	{
		if (GAMEINSTANCE->Get_DIMouseKeyState(MOUSEBUTTON::MBS_WHEEL) & 0x80)
		{
			m_bIsFocused = !m_bIsFocused;
			if (m_bIsFocused)
				GET_SINGLE(CGameManager)->Focus_Monster();
			else
				GET_SINGLE(CGameManager)->Release_Focus();
		}

		if (m_bIsFocused)
		{
			if (!m_pTargetMonster.lock() || m_pTargetMonster.lock()->Get_Dead())
			{
				return;
			}
			Look_At_Target(fTimeDelta);
		}
		else
		{
			Free_MouseMove(fTimeDelta);
		}

		Interpolate_Camera(fTimeDelta);
	}

}

void CCamera_Target::LateTick(_float fTimeDelta)
{

	Bind_PipeLine();
}

HRESULT CCamera_Target::Render()
{
	return S_OK;
}

void CCamera_Target::Change_Target()
{
	/*추후에 시간 나면 타겟 바꾸는 기능 추가*/
}

void CCamera_Target::Focus_Monster(weak_ptr<CGameObject> _pMonster)
{
	m_pTargetMonster = _pMonster;
	m_pTargetMonsterTransformCom = _pMonster.lock()->Get_Component<CTransform>();

}

void CCamera_Target::Release_Focus()
{
	m_pTargetMonster = weak_ptr<CGameObject>();
	m_pTargetMonsterTransformCom = weak_ptr<CTransform>();

}

void CCamera_Target::Start_Cinematic(weak_ptr<CModel> _pModel, const _char* pBoneName)
{
	m_pCameraBoneNode = _pModel.lock()->Find_BoneNode(pBoneName);
	m_pCameraBoneParentTransform = _pModel.lock()->Get_Owner().lock()->Get_Component<CTransform>();
	m_TransformationMatrix = _pModel.lock()->Get_TransformationMatrix();
	m_bCinematic = true;

	XMStoreFloat4x4(&m_OriginalMatrix, m_pTransformCom.lock()->Get_WorldMatrix());
}

void CCamera_Target::End_Cinematic()
{

	_matrix LocalMat = XMMatrixIdentity();
	LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
	LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));

	_matrix		ParentMatrix = m_pCameraBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&m_TransformationMatrix);

	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	_matrix TotalMatrix = LocalMat * ParentMatrix * m_pCameraBoneParentTransform.lock()->Get_WorldMatrix();

	m_pTransformCom.lock()->Set_WorldMatrix(TotalMatrix);
	_vector vCurPlayerPos = m_pCurrentPlayerTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	XMStoreFloat4(&m_vPrePlayerPos, vCurPlayerPos);
	XMStoreFloat4(&m_vPlayerFollowLerpPosition, vCurPlayerPos);

	m_pCameraBoneNode = weak_ptr<CBoneNode>();
	m_pCameraBoneParentTransform = weak_ptr<CTransform>();
	m_bCinematic = false;

}


HRESULT CCamera_Target::Bind_PipeLine()
{
	_matrix WorldMatrix;
	if (!m_bCinematic)
	{
		GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_WORLD, m_pTransformCom.lock()->Get_WorldMatrix());
		WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
	}
	else
	{
		WorldMatrix = XMLoadFloat4x4(&m_CinemaWorldMatrix);
		GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_WORLD, WorldMatrix);
	}
	_matrix		ViewMatrix = XMMatrixInverse(nullptr, WorldMatrix);
	GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);

	_matrix		ProjMatrix = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);

	return S_OK;
}

void CCamera_Target::Look_At_Target(_float fTimeDelta)//타겟 고정
{
	_vector vPlayerPos = m_pCurrentPlayerTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPos = m_pTargetMonsterTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vLookDir = XMVector3Normalize(vTargetPos - vPlayerPos - XMVectorSet(0.f, 1.5f, 0.f, 0.f));

	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookDir);
	_vector vUp = XMVector3Cross(vLookDir, vRight);

	_matrix vLookTargetMatrix;
	vLookTargetMatrix.r[0] = vRight;
	vLookTargetMatrix.r[1] = vUp;
	vLookTargetMatrix.r[2] = vLookDir;
	vLookTargetMatrix.r[3] = vPlayerPos;

	_vector vLookTargetQuaternion = XMQuaternionRotationMatrix(vLookTargetMatrix);
	_vector vCurCameraQuaternion = XMQuaternionRotationMatrix(m_pTransformCom.lock()->Get_WorldMatrix());


	//m_fRotationLerpRatio를 위 처럼 일정 비율 증가하게 해야함 조건은 각도가 일정 크기 이상으로 되었을 때
	//ratio를 증가 시키고 아닐 때 0으로 만들어 놓음 <- 조건을 찾아야함
	_vector vLerpQuaternion = XMQuaternionSlerp(vCurCameraQuaternion, vLookTargetQuaternion, 2.f*fTimeDelta);

	m_pTransformCom.lock()->Rotation_Quaternion(vLerpQuaternion);
}

void CCamera_Target::Free_MouseMove(_float fTimeDelta)//마우스 움직임
{
	_long		MouseMove = 0;

	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
	{
		m_iMouseMovementX += MouseMove;
	}

	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
	{
		m_iMouseMovementY += MouseMove;
	}
	if (fabs(m_iMouseMovementY) > DBL_EPSILON)
		m_pTransformCom.lock()->Turn(m_pTransformCom.lock()->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_iMouseMovementY * 0.2f * 0.1f);
	m_iMouseMovementY *= 0.8f;

	if (fabs(m_iMouseMovementX) > DBL_EPSILON)
		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 0.1f * m_iMouseMovementX * 0.2f);
	m_iMouseMovementX *= 0.8f;

}


void CCamera_Target::Interpolate_Camera(_float fTimeDelta)//항상 적용
{
	_vector vPlayerPos = m_pCurrentPlayerTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPrePlayerPos = XMLoadFloat4(&m_vPrePlayerPos);
	XMStoreFloat4(&m_vPrePlayerPos, vPlayerPos);


	if (fTimeDelta < XMVector3Length(vPlayerPos - vPrePlayerPos).m128_f32[0])
	{
		_vector  vPlayerFollowLerpPostiion = XMLoadFloat4(&m_vPlayerFollowLerpPosition);
		
		m_fSpeed += m_fAccel * fTimeDelta;
		if (5.f < m_fSpeed)
			m_fSpeed = 5.f;

		vPlayerFollowLerpPostiion = XMVectorLerp(vPlayerFollowLerpPostiion, vPrePlayerPos, m_fSpeed * fTimeDelta);
		XMStoreFloat4(&m_vPlayerFollowLerpPosition, vPlayerFollowLerpPostiion);
	}
	else
	{
		m_fSpeed = 0.f;

		_vector vPlayerFollowLerpPos = XMLoadFloat4(&m_vPlayerFollowLerpPosition);
		vPlayerFollowLerpPos.m128_f32[3] = 1.f;

		vPlayerFollowLerpPos = XMVectorLerp(vPlayerFollowLerpPos, vPrePlayerPos, fTimeDelta);
		XMStoreFloat4(&m_vPlayerFollowLerpPosition, vPlayerFollowLerpPos);
	}

	_vector vLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	_vector vPos = XMLoadFloat4(&m_vPlayerFollowLerpPosition) + vLook * -4.5f + XMVectorSet(0.f, 1.1f, 0.f, 0.f);
	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vPos);
}

void CCamera_Target::Update_Bone()
{
	//Varg_Execution camera LocalOffset.
	_matrix LocalMat = XMMatrixIdentity();
	LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
	LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));

	_matrix		ParentMatrix = m_pCameraBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&m_TransformationMatrix);

	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	_matrix TotalMatrix = LocalMat *ParentMatrix * m_pCameraBoneParentTransform.lock()->Get_WorldMatrix();

	XMStoreFloat4x4(&m_CinemaWorldMatrix, TotalMatrix);
	
}

void CCamera_Target::OnLevelExit()
{
	m_bFirst = true;
}

void CCamera_Target::Free()
{
}
