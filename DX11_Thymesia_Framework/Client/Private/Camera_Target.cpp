#include "stdafx.h"

#include "Camera_Target.h"
#include "GameManager.h"
#include "Monster.h"
#include "Player.h"
#include "GameObject.h"
#include "Model.h"
#include "BoneNode.h"
#include "Easing_Utillity.h"
#include "PhysXCameraController.h"
#include "Preset_PhysXControllerDesc.h"
#include "Transform.h"

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

	m_pPhysXCameraControllerCom = Add_Component<CPhysXCameraController>();
	m_pPhysXCameraControllerCom.lock()->Set_CurrentCameraController();
	m_pPhysXCameraControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::CameraSetting(m_pTransformCom),
		(_uint)PHYSX_COLLISION_LAYER::CAMERA);

	GET_SINGLE(CGameManager)->Registration_Camera("Camera_Target", Weak_Cast<CCamera>(m_this));

	m_bMouseMoveEnable = true;

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
	_vector vPlayerPos = m_pCurrentPlayerTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	XMStoreFloat4(&m_vPlayerFollowLerpPosition, vPlayerPos);
	XMStoreFloat4(&m_vPrePlayerPos, vPlayerPos);

	GET_SINGLE(CGameManager)->Use_EffectGroup("GlobalDust", m_pTransformCom);

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

	if (m_bStop)
	{
		return;
	}


	if (m_bCinematic)
	{
		Update_Bone();
	}
	else
	{
		if (KEY_INPUT(KEY::MBUTTON, KEY_STATE::TAP))
		{
			if (m_bIsFocused)
			{
				GET_SINGLE(CGameManager)->Release_Focus();
				//TODO: 임시 쉐이킹 기본 타격 셰이킹은 이거 쓰면 될 듯
				//GET_SINGLE(CGameManager)->Add_Shaking(XMVectorSet(0.f, 1.f, 1.f, 0.f), 0.5f,0.1f);
				//TODO: 과격한 타격 쉐이킹
				// GET_SINGLE(CGameManager)->Add_Shaking(XMVectorSet(0.f, 1.f, 1.f, 0.f), 0.5f,0.6f);

				//GET_SINGLE(CGameManager)->Activate_Zoom(-1.f);
			}
			else
			{
				GET_SINGLE(CGameManager)->Focus_Monster();
				//TODO: 과격한 타격 쉐이킹
				// GET_SINGLE(CGameManager)->Add_Shaking(XMVectorSet(0.f, 1.f, 1.f, 0.f), 0.5f, 0.6f);
				//GET_SINGLE(CGameManager)->Deactivate_Zoom();
			}
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
			if (m_bMouseMoveEnable == true)
			{
				Free_MouseMove(fTimeDelta);
			}

		}
		Calculate_ShakingOffSet(fTimeDelta);
		Calculate_ZoomOffSet(fTimeDelta);
		Interpolate_Camera(fTimeDelta);
	}



}

void CCamera_Target::LateTick(_float fTimeDelta)
{
	Update_PhysXCollider(fTimeDelta);
	
	Bind_PipeLine();
}

HRESULT CCamera_Target::Render(ID3D11DeviceContext* pDeviceContext)
{
	return S_OK;
}

void CCamera_Target::Change_Target()
{
	/*추후에 시간 나면 타겟 바꾸는 기능 추가*/
}

void CCamera_Target::Focus_Monster(weak_ptr<CMonster> _pMonster)
{
	if (!_pMonster.lock())
		return;

	m_bIsFocused = true;
	m_pTargetMonster = _pMonster;
	m_pTargetMonsterTransformCom = _pMonster.lock()->Get_Transform();

	m_pTargetMonsterModelCom = _pMonster.lock()->Get_Model();

	m_pTargetMonsterBoneNodeCom = m_pTargetMonsterModelCom.lock()->Find_BoneNode("Spine01");
	if (!m_pTargetMonsterBoneNodeCom.lock())
	{
		m_pTargetMonsterBoneNodeCom = m_pTargetMonsterModelCom.lock()->Find_BoneNode("Bip001-Spine1");
	}
	if (!m_pTargetMonsterBoneNodeCom.lock())
	{
		m_pTargetMonsterBoneNodeCom = m_pTargetMonsterModelCom.lock()->Find_BoneNode("spine_01");
	}
	if (!m_pTargetMonsterBoneNodeCom.lock())
	{
		m_pTargetMonsterBoneNodeCom = m_pTargetMonsterModelCom.lock()->Find_BoneNode("Bone");
	}

}

void CCamera_Target::Release_Focus()
{
	m_bIsFocused = false;
	m_pTargetMonster = weak_ptr<CGameObject>();
	m_pTargetMonsterTransformCom = weak_ptr<CTransform>();
	m_pTargetMonsterModelCom = weak_ptr<CModel>();
	m_pTargetMonsterBoneNodeCom = weak_ptr<CBoneNode>();
}

void CCamera_Target::Start_Cinematic(weak_ptr<CModel> _pModel, const _char* pBoneName, _fmatrix OffSetMatrix, CINEMATIC_TYPE eType)
{

	m_iMouseMovementX = 0;
	m_iMouseMovementY = 0;

	m_pCameraBoneNode = _pModel.lock()->Find_BoneNode(pBoneName);
	m_pCameraBoneParentTransform = _pModel.lock()->Get_Owner().lock()->Get_Component<CTransform>();
	m_TransformationMatrix = _pModel.lock()->Get_TransformationMatrix();
	m_bCinematic = true;

	m_eCinematicType = eType;

	XMStoreFloat4x4(&m_CinematicOffSetMatrix, OffSetMatrix);

	Update_Bone();

}

void CCamera_Target::End_Cinematic()
{
	if (m_eCinematicType == CINEMATIC_TYPE::EXECUTION)
	{
		_matrix		ParentMatrix = m_pCameraBoneNode.lock()->Get_CombinedMatrix()
			* XMLoadFloat4x4(&m_TransformationMatrix);

		ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
		ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
		ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

		_matrix TotalMatrix = XMLoadFloat4x4(&m_CinematicOffSetMatrix) * ParentMatrix * m_pCameraBoneParentTransform.lock()->Get_WorldMatrix();

		m_pTransformCom.lock()->Set_WorldMatrix(TotalMatrix);
		_vector vLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
		_vector vCurPlayerPos = m_pCurrentPlayerTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
		XMStoreFloat4(&m_vPlayerFollowLerpPosition, TotalMatrix.r[3] + vLook * 3.25f - XMVectorSet(0.f, 1.1f, 0.f, 0.f));
		XMStoreFloat4(&m_vPrePlayerPos, vCurPlayerPos);

		m_pCameraBoneNode = weak_ptr<CBoneNode>();
		m_pCameraBoneParentTransform = weak_ptr<CTransform>();

	}
	else if (m_eCinematicType == CINEMATIC_TYPE::CINEMATIC)
	{
		_matrix PlayerMatrix = m_pCurrentPlayerTransformCom.lock()->Get_WorldMatrix();

		PlayerMatrix.r[0] = XMVector3Normalize(PlayerMatrix.r[0]);
		PlayerMatrix.r[1] = XMVector3Normalize(PlayerMatrix.r[1]);
		PlayerMatrix.r[2] = XMVector3Normalize(PlayerMatrix.r[2]);

		_vector vLook = PlayerMatrix.r[2];
		PlayerMatrix.r[3] = PlayerMatrix.r[3] + vLook * -3.5f + XMVectorSet(0.f, 1.1f, 0.f, 0.f);
		m_pTransformCom.lock()->Set_WorldMatrix(PlayerMatrix);

		/*그냥 대충 페이드아웃*/
		m_pCameraBoneNode = weak_ptr<CBoneNode>();
		m_pCameraBoneParentTransform = weak_ptr<CTransform>();

	}
	m_bCinematic = false;
}

void CCamera_Target::Activate_Zoom(_float fRatio, _float fZoomTime, EASING_TYPE eZoomLerpFunc, DIRECTION eMoveDir)
{
	m_fZoomEndOffSet = fRatio;
	m_fZoomStartOffSet = m_fZoom;
	m_fZoomTimeAcc = 0.f;
	m_fZoomTime = fZoomTime;
	m_eZoomLerpFunc = eZoomLerpFunc;
	m_eMoveDirection = eMoveDir;
}

void CCamera_Target::Deactivate_Zoom(_float fZoomTime, EASING_TYPE eZoomLerpFunc)
{
	m_fZoomStartOffSet = m_fZoom;
	m_fZoomEndOffSet = 0.f;
	m_fZoomTime = fZoomTime;
	m_fZoomTimeAcc = 0.f;
	m_eZoomLerpFunc = eZoomLerpFunc;
	
}

void CCamera_Target::Add_Shaking(_vector vShakingDir, _float fRatio, _float fShakingTime, _float fFrequency, _float fDecreaseRatio)
{
	if (0.5f < vShakingDir.m128_f32[3])
		m_bRandomShaking = true;
	else
	{
		vShakingDir = XMVector3TransformNormal(vShakingDir, m_pTransformCom.lock()->Get_WorldMatrix());
		vShakingDir = XMVector3Normalize(vShakingDir);
		XMStoreFloat3(&m_vShakingDir, vShakingDir);
		m_bRandomShaking = false;
	}

	m_vShakingStartOffSet = m_vShaking;
	m_fShakingTime = fShakingTime;
	m_fShakingFrequency = fFrequency;

	m_fShakeRatio = fRatio;
	m_fDecreaseRatio = fDecreaseRatio;
	m_fShakingTimeAcc = 0.f;
	m_fShakingQuarterFrequency = 0.f;
	m_fShakingDecreaseTime = 0.f;

}


HRESULT CCamera_Target::Bind_PipeLine()
{
	_matrix WorldMatrix;
	if (!m_bCinematic)
	{
		if (m_bCollision)
		{
			WorldMatrix = XMLoadFloat4x4(&m_CollisionMatrix);
		}
		else
		{
			WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
		}

		GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_WORLD, m_pTransformCom.lock()->Get_WorldMatrix());
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
	_float fLerpRatio(5.f * fTimeDelta);
	if (1.f < fLerpRatio)
		fLerpRatio = 1.f;

	//_float4x4 matTargetModelTransformationMatrix = m_pTargetMonsterModelCom.lock()->Get_TransformationMatrix();
	//_matrix  matTargetMonsterWorld = XMLoadFloat4x4(&matTargetModelTransformationMatrix);
	//_matrix  matTargetCombined = m_pTargetMonsterBoneNodeCom.lock()->Get_CombinedMatrix();


	//_matrix BoneMatrix = matTargetCombined * matTargetMonsterWorld;

	//BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	//BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	//BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	//_matrix MonsterWorldMatrix = BoneMatrix * m_pTargetMonsterTransformCom.lock()->Get_UnScaledWorldMatrix();


	_vector vPlayerPos = m_pCurrentPlayerTransformCom.lock()->Get_Position() /*+ XMVectorSet(0.f,1.f,0.f,0.f)*/;
	_vector vTargetPos = m_pTargetMonsterTransformCom.lock()->Get_Position();
	_vector vLookDir = XMVector3Normalize(vTargetPos - vPlayerPos);

	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookDir);
	_vector vUp = XMVector3Cross(vLookDir, vRight);

	_matrix vLookTargetMatrix;

	_matrix RotationMatrix = XMMatrixRotationAxis(vRight, XMConvertToRadians(25.f));

	vLookTargetMatrix.r[0] = XMVector3TransformNormal(vRight, RotationMatrix);
	vLookTargetMatrix.r[1] = XMVector3TransformNormal(vUp, RotationMatrix);
	vLookTargetMatrix.r[2] = XMVector3TransformNormal(vLookDir, RotationMatrix);
	vLookTargetMatrix.r[3] = vPlayerPos;

	_vector vLookTargetQuaternion = XMQuaternionRotationMatrix(vLookTargetMatrix);
	_vector vCurCameraQuaternion = XMQuaternionRotationMatrix(m_pTransformCom.lock()->Get_WorldMatrix());

	_vector vLerpQuaternion = XMQuaternionSlerp(vCurCameraQuaternion, vLookTargetQuaternion, fLerpRatio);

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

	_matrix RotationMatrix = XMMatrixRotationAxis(m_pTransformCom.lock()->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_iMouseMovementY * 0.2f * 0.1f);
	_vector vLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	_vector vPlayerUp = m_pCurrentPlayerTransformCom.lock()->Get_State(CTransform::STATE_UP);
	_float fDotValue = XMVector3Dot(vLook, vPlayerUp).m128_f32[0];
	if (0.9f > fDotValue && -0.99f < fDotValue)
	{
		if (fabs(m_iMouseMovementY) > DBL_EPSILON)
			m_pTransformCom.lock()->Turn(m_pTransformCom.lock()->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_iMouseMovementY * 0.2f * 0.1f);
		m_iMouseMovementY = _long(m_iMouseMovementY * 0.8f);
	}
	else
	{
		m_iMouseMovementY = 0;
	}

	if (fabs(m_iMouseMovementX) > DBL_EPSILON)
		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 0.1f * m_iMouseMovementX * 0.2f);

	m_iMouseMovementX = _long(m_iMouseMovementX * 0.8f);

}

void CCamera_Target::Calculate_ZoomOffSet(_float fTimeDelta)
{
	if (m_fZoomTime > fTimeDelta)
	{
		m_fZoomTimeAcc += fTimeDelta;
		m_fZoomTimeAcc = min(m_fZoomTimeAcc, m_fZoomTime);
	}
 

	if (fabs(m_fZoomEndOffSet - m_fZoom)> DBL_EPSILON)
	{
		_vector vStartPoint = XMVectorSet(m_fZoomStartOffSet, 0.f, 0.f, 0.f);
		_vector vEndPoint = XMVectorSet(m_fZoomEndOffSet, 0.f, 0.f, 0.f);

		m_fZoom = CEasing_Utillity::LerpToType(vStartPoint, vEndPoint, m_fZoomTimeAcc, m_fZoomTime, m_eZoomLerpFunc).m128_f32[0];
	}


}

void CCamera_Target::Calculate_ShakingOffSet(_float fTimeDelta)
{

	m_fShakingTimeAcc += fTimeDelta;
	m_fShakingTime -= fTimeDelta;


	if (m_fShakingTime > DBL_EPSILON)
	{
		_float fRadian = m_fShakingTimeAcc;
		if (m_bRandomShaking)
		{
			m_vShakingDir = SMath::vRandom(_float3(-1.f, -1.f, -1.f), _float3(1.f, 1.f, 1.f));
			XMStoreFloat3(&m_vShakingDir, XMVector3Normalize(XMLoadFloat3(&m_vShakingDir)));

			XMStoreFloat3(&m_vShaking, m_fShakeRatio * XMLoadFloat3(&m_vShakingDir));
			m_fShakeRatio *= m_fDecreaseRatio;
		}
		else
		{
			XMStoreFloat3(&m_vShaking, sinf(fRadian * m_fShakingFrequency * XM_2PI) * m_fShakeRatio * XMLoadFloat3(&m_vShakingDir));

			m_fShakingQuarterFrequency += fTimeDelta;
			if (m_fShakingQuarterFrequency > 1 / (m_fShakingFrequency) * 0.5f)
			{
				m_fShakeRatio *= m_fDecreaseRatio;
				m_fShakingQuarterFrequency = 0.f;
			}
		}
		m_vShakingStartOffSet = m_vShaking;

	}
	else
	{
		m_fShakingDecreaseTime += fTimeDelta;
		if (1.5f > m_fShakingDecreaseTime)
		{
			_vector vStartPoint = XMLoadFloat3(&m_vShakingStartOffSet);
			_vector vEndPoint = XMVectorSet(0.f, 0.f, 0.f, 0.f);

			XMStoreFloat3(&m_vShaking, CEasing_Utillity::CircOut(vStartPoint, vEndPoint, m_fShakingDecreaseTime, 1.5f));
			//XMStoreFloat3(&m_vShaking, XMVectorLerp(XMLoadFloat3(&m_vShaking), XMVectorSet(0.f, 0.f, 0.f, 0.f), fTimeDelta*fTimeDelta));
		}

		m_bRandomShaking = false;
	}


}


void CCamera_Target::Interpolate_Camera(_float fTimeDelta)//항상 적용
{
	_vector vPlayerPos = m_pCurrentPlayerTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPrePlayerPos = XMLoadFloat4(&m_vPrePlayerPos);
	XMStoreFloat4(&m_vPrePlayerPos, vPlayerPos);

	_float fDistance = XMVector3Length(vPlayerPos - vPrePlayerPos).m128_f32[0];

	if (fTimeDelta < fDistance)
	{
		_vector  vPlayerFollowLerpPostiion = XMLoadFloat4(&m_vPlayerFollowLerpPosition);

		m_fSpeed += m_fAccel * fTimeDelta;

		if (5.f + fDistance < m_fSpeed)
			m_fSpeed = 5.f + fDistance;

		//플레이어와 카메라 사이 거리에 따라서 거리 이동 보간 비율을 크게 주기

		vPlayerFollowLerpPostiion = XMVectorLerp(vPlayerFollowLerpPostiion, vPrePlayerPos, m_fSpeed * fTimeDelta);
		XMStoreFloat4(&m_vPlayerFollowLerpPosition, vPlayerFollowLerpPostiion);
	}
	else
	{
		m_fSpeed = 0.f;

		_vector vPlayerFollowLerpPos = XMLoadFloat4(&m_vPlayerFollowLerpPosition);
		vPlayerFollowLerpPos.m128_f32[3] = 1.f;

		vPlayerFollowLerpPos = XMVectorLerp(vPlayerFollowLerpPos, vPrePlayerPos, 2.f * fTimeDelta);
		XMStoreFloat4(&m_vPlayerFollowLerpPosition, vPlayerFollowLerpPos);
	}

	_vector vZoomDir = m_pTransformCom.lock()->Get_State((CTransform::STATE)m_eMoveDirection);
	_vector vLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	_vector vPos = XMLoadFloat4(&m_vPlayerFollowLerpPosition) + vLook * -3.25f + vZoomDir*m_fZoom + XMVectorSet(0.f, 1.1f, 0.f, 0.f) + XMLoadFloat3(&m_vShaking);
	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vPos);

	_float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(m_pTransformCom.lock()->Get_WorldMatrix()));
	vPitchYawRoll.z = 0.f;

	m_pTransformCom.lock()->Rotation_PitchYawRoll(vPitchYawRoll);
}

void CCamera_Target::Update_Bone()
{
	_matrix		ParentMatrix = m_pCameraBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&m_TransformationMatrix);

	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	_matrix TotalMatrix = XMLoadFloat4x4(&m_CinematicOffSetMatrix) * ParentMatrix * m_pCameraBoneParentTransform.lock()->Get_WorldMatrix();

	XMStoreFloat4x4(&m_CinemaWorldMatrix, TotalMatrix);

}

void CCamera_Target::Update_PhysXCollider(_float fTimeDelta)
{
	if (!m_pCurrentPlayerTransformCom.lock())
		return;

	PxControllerFilters Filters;

	m_pPhysXCameraControllerCom.lock()->Update_Ray(m_pCurrentPlayerTransformCom);
	m_pPhysXCameraControllerCom.lock()->Synchronize_Controller(m_pTransformCom, fTimeDelta, Filters);
	m_pPhysXCameraControllerCom.lock()->Update_RayCastCollision(fTimeDelta);

	m_bCollision = m_pPhysXCameraControllerCom.lock()->Is_Collision();

	if (m_bCollision)
	{
		_vector vCollisionPosition = m_pPhysXCameraControllerCom.lock()->Get_Position();

		_matrix vCollisionMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
		vCollisionMatrix.r[3] = vCollisionPosition;

		XMStoreFloat4x4(&m_CollisionMatrix, vCollisionMatrix);
	}
}
void CCamera_Target::OnEventMessage(_uint iArg)
{
	if (EVENT_TYPE::ON_RESPAWN == (EVENT_TYPE)iArg)
	{
		_vector vCurPlayerPos = m_pCurrentPlayer.lock()->Get_WorldPosition();

		XMStoreFloat4(&m_vPlayerFollowLerpPosition, vCurPlayerPos);
		XMStoreFloat4(&m_vPrePlayerPos, vCurPlayerPos);
	}
}

void CCamera_Target::OnLevelExit()
{
	m_bFirst = true;
}

void CCamera_Target::Free()
{
}
