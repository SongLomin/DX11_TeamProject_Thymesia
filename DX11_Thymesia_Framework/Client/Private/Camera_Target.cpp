#include "stdafx.h"
#include "Camera_Target.h"
#include "GameManager.h"
#include "Monster.h"
#include "Player.h"
#include "GameObject.h"

GAMECLASS_C(CCamera_Target);
CLONE_C(CCamera_Target, CGameObject);

CCamera_Target::CCamera_Target(const CCamera_Target& rhs)
{
	*this = rhs;
}

HRESULT CCamera_Target::Initialize_Prototype()
{

	m_tLuxiyaUltimateKeys.reserve(2);

	CINEMA_DESC tCinemaKey;
	ZeroMemory(&tCinemaKey, sizeof(CINEMA_DESC));

	// 루시아 궁극기
	tCinemaKey.vPitchYawRoll = {0.f, 0.f, 0.f};
	tCinemaKey.vOffset = { 0.f, 1.f, 0.f };
	tCinemaKey.fDistance = 1.2f;
	tCinemaKey.fTime = 0.f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tLuxiyaUltimateKeys.emplace_back(tCinemaKey);

	tCinemaKey.vPitchYawRoll = { -0.2f, 2.f, 0.f };
	tCinemaKey.vOffset = { 0.f, 0.8f, 0.f };
	tCinemaKey.fDistance = 0.85f;
	tCinemaKey.fTime = 0.7f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tLuxiyaUltimateKeys.emplace_back(tCinemaKey);

	tCinemaKey.vPitchYawRoll = { -0.2f, 2.14f, 0.f };
	tCinemaKey.vOffset = { 0.f, 0.8f, 0.f };
	tCinemaKey.fDistance = 0.85f;
	tCinemaKey.fTime = 0.5f;
	tCinemaKey.eInterpolationType = INTERPOLATION::LINEAR;
	m_tLuxiyaUltimateKeys.emplace_back(tCinemaKey);

	// 승리 연출
	tCinemaKey.vPitchYawRoll = { 0.f, 3.f, 0.f };
	tCinemaKey.vOffset = { 0.f, 1.3f, 0.f };
	tCinemaKey.fDistance = 1.2f;
	tCinemaKey.fTime = 0.f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tBattleEndCinemaKeys.emplace_back(tCinemaKey);

	tCinemaKey.vPitchYawRoll = { 0.f, 3.4f, 0.f };
	tCinemaKey.vOffset = { 0.f, 1.3f, 0.f };
	tCinemaKey.fDistance = 1.2f;
	tCinemaKey.fTime = 4.2f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tBattleEndCinemaKeys.emplace_back(tCinemaKey);

	tCinemaKey.vPitchYawRoll = { 0.f, 3.4f, 0.f };
	tCinemaKey.vOffset = { 0.f, 1.3f, 0.f };
	tCinemaKey.fDistance = 1.2f;
	tCinemaKey.fTime = 999.f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tBattleEndCinemaKeys.emplace_back(tCinemaKey);

	// 루시아 시작 연출
	tCinemaKey.vPitchYawRoll = { 0.f, 3.14f, 0.f };
	tCinemaKey.vOffset = { 0.f, 3.f, 0.f };
	tCinemaKey.fDistance = 1.2f;
	tCinemaKey.fTime = 0.f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tLuxiyaBornKeys.emplace_back(tCinemaKey);

	tCinemaKey.vPitchYawRoll = { -0.1f, 3.14f, 0.f };
	tCinemaKey.vOffset = { 0.f, 1.f, 0.f };
	tCinemaKey.fDistance = 3.f;
	tCinemaKey.fTime = 0.5f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tLuxiyaBornKeys.emplace_back(tCinemaKey);

	tCinemaKey.vPitchYawRoll = { -0.1f, 3.14f, 0.f };
	tCinemaKey.vOffset = { 0.f, 1.f, 0.f };
	tCinemaKey.fDistance = 3.f;
	tCinemaKey.fTime = 0.8385f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tLuxiyaBornKeys.emplace_back(tCinemaKey);

	tCinemaKey.vPitchYawRoll = { 0.f, 3.14f, 0.f };
	tCinemaKey.vOffset = { 0.f, 1.3f, 0.f };
	tCinemaKey.fDistance = 1.2f;
	tCinemaKey.fTime = 0.7f;
	tCinemaKey.eInterpolationType = INTERPOLATION::POW;
	m_tLuxiyaBornKeys.emplace_back(tCinemaKey);

	tCinemaKey.fTime = 1.7f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tLuxiyaBornKeys.emplace_back(tCinemaKey);

	// 보스 등장 연출
	tCinemaKey.vPitchYawRoll = { 0.f, 3.14f, 0.f };
	tCinemaKey.vOffset = { 0.f, 3.f, 0.f };
	tCinemaKey.fDistance = 5.f;
	tCinemaKey.fTime = 0.f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tBossStartCinemaKeys.emplace_back(tCinemaKey);

	tCinemaKey.vPitchYawRoll = { -0.1f, 3.14f, 0.f };
	tCinemaKey.vOffset = { 0.f, 1.f, 0.f };
	tCinemaKey.fDistance = 3.f;
	tCinemaKey.fTime = 3.2f;
	tCinemaKey.eInterpolationType = INTERPOLATION::LINEAR;
	m_tBossStartCinemaKeys.emplace_back(tCinemaKey);

	tCinemaKey.vPitchYawRoll = { 0.f, 3.14f, 0.f };
	tCinemaKey.vOffset = { 0.f, 3.f, 0.f };
	tCinemaKey.fDistance = 7.f;
	tCinemaKey.fTime = 0.6f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tBossStartCinemaKeys.emplace_back(tCinemaKey);

	tCinemaKey.vPitchYawRoll = { 0.f, 3.14f, 0.f };
	tCinemaKey.vOffset = { 0.f, 3.f, 0.f };
	tCinemaKey.fDistance = 7.f;
	tCinemaKey.fTime = 1.7f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tBossStartCinemaKeys.emplace_back(tCinemaKey);

	/*tCinemaKey.vPitchYawRoll = { 0.f, 3.14f, 0.f };
	tCinemaKey.vOffset = { 0.f, 1.3f, 0.f };
	tCinemaKey.fDistance = 3.f;
	tCinemaKey.fTime = 0.7f;
	tCinemaKey.eInterpolationType = INTERPOLATION::POW;
	m_tBossStartCinemaKeys.emplace_back(tCinemaKey);*/

	tCinemaKey.fTime = 4.8f;
	tCinemaKey.eInterpolationType = INTERPOLATION::SQRT;
	m_tBossStartCinemaKeys.emplace_back(tCinemaKey);




	return S_OK;
}

HRESULT CCamera_Target::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pTransformCom.lock()->Add_Position(XMVectorSet(0.f, 2.f, 0.f, 1.f));

	m_vTargetPosition = _float3(0.f, 0.f, 0.f);
	m_vTargetQuaternion = _float4(0.f, 0.f, 0.f, 0.f);

	_matrix MatLookAtZeroPoint = XMMatrixIdentity();
	MatLookAtZeroPoint.r[3] = XMVectorSet(0.f, 3.5f, -3.5f, 1.f);
	MatLookAtZeroPoint = SMath::LookAt(MatLookAtZeroPoint, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	_vector vLookAtZeroPointQuat = XMQuaternionRotationMatrix(SMath::Get_RotationMatrix(MatLookAtZeroPoint));


	return S_OK;
}

HRESULT CCamera_Target::Start()
{


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

	if (m_bCinema)
	{
		Update_Cinema(fTimeDelta);
		return;
	}

	Bake_TargetCamera(fTimeDelta);

	if (KEY_INPUT(KEY::Q, KEY_STATE::HOLD))
	{
		m_fDistance -= fTimeDelta * 5.f;
		//m_pTransformCom.lock()->Go_Straight(fTimeDelta * 15.f);
	}

	if (KEY_INPUT(KEY::E, KEY_STATE::HOLD))
	{
		m_fDistance += fTimeDelta * 5.f;
		//m_pTransformCom.lock()->Go_Backward(fTimeDelta * 15.f);
	}

	Interpolate_Camera(fTimeDelta);

	Tick_Shaking(fTimeDelta);
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
	m_pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	//현재 플레이어가 NULLPTR임.
	if (!m_pCurrentPlayer.lock().get())
		DEBUG_ASSERT;

	m_pCurrentPlayerTransformCom = m_pCurrentPlayer.lock()->Get_Component<CTransform>();

	m_pLookTargetMonster = m_pCurrentPlayer.lock()->Get_TargetMonster();

	m_fCurFreqTime = 0.f;
}

void CCamera_Target::Add_Shaking(const SHAKE_DIRECTION& In_eState, const _float& In_fPower, const _float& In_fTime)
{
	m_eCurrentShakeDir = In_eState;
	m_fShakePower = In_fPower;
	m_fMaxShakeTime = In_fTime;
	m_fCurrentShakeTime = m_fLoopShakeTime * 0.5f;
	m_bReverseShake = false;
}

//void CCamera_Target::Set_Target(weak_ptr<CGameObject> In_Target)
//{
//	m_pCamTargetObject = In_Target;
//	m_pCamTargetTransformCom = In_Target.lock()->Get_Component<CTransform>();
//}

HRESULT CCamera_Target::Bind_PipeLine()
{
	GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_WORLD, XMLoadFloat4x4(&m_MatResult));

	//_matrix		WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();

	_matrix WorldMatrix = XMLoadFloat4x4(&m_MatResult);

	/*_matrix		WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix()
		* XMLoadFloat4x4(&m_MatRotation)
		* SMath::Get_PositionMatrix(m_pCamTargetTransformCom.lock()->Get_WorldMatrix());
	GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_WORLD, WorldMatrix);*/

	_matrix		ViewMatrix = XMMatrixInverse(nullptr, WorldMatrix);
	GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);

	_matrix		ProjMatrix = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);

	return S_OK;
}

void CCamera_Target::Init_TargetJustPlayer()
{
	_matrix MyMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
	_vector vPlayerPosition = m_pCurrentPlayerTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

	MyMatrix = SMath::LookAt(MyMatrix, vPlayerPosition);

	_float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(MyMatrix));
	m_fXAxisRadian = vPitchYawRoll.x;
	m_fYAxisRadian = vPitchYawRoll.y;
	m_fTargetPlayerCamLerpTime = 0.f;
}

void CCamera_Target::Bake_TargetCamera(_float fTimeDelta)
{
	m_pLookTargetMonster = m_pCurrentPlayer.lock()->Get_TargetMonster();


	if (KEY_INPUT(KEY::TAB, KEY_STATE::TAP))
	{
		m_isMonsterTarget = !m_isMonsterTarget;
		//m_eNextCameraState = m_isMonsterTarget ? CAMERA_STATE::LOOKMONSTER : CAMERA_STATE::LOOKPLAYER;
		//m_eCameraState = CAMERA_STATE::INTERPOLATE;
		m_eCameraState = m_isMonsterTarget ? CAMERA_STATE::LOOKMONSTER : CAMERA_STATE::LOOKPLAYER;

		if (CAMERA_STATE::LOOKPLAYER == m_eCameraState)
		{
			Init_TargetJustPlayer();
		}

	}

	//타겟 몬스터가 있을 경우
	if (m_isMonsterTarget && m_pLookTargetMonster.lock())
		//if (m_pLookTargetMonster.lock().get())
		Bake_TargetWithMonster(fTimeDelta);
	//타겟 몬스터가 없을 경우
	else
		Bake_TargetJustPlayer(fTimeDelta);


}

void CCamera_Target::Bake_TargetWithMonster(_float fTimeDelta)
{

	_vector vMonsterPosition = m_pLookTargetMonster.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPlayerPosition = m_pCurrentPlayerTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

	_matrix PlayerToMonsterMatrix;

	_vector vLook = vMonsterPosition - vPlayerPosition;

	PlayerToMonsterMatrix = SMath::Bake_MatrixNormalizeUseLookVector(vLook);
	
	m_fDistance = XMVector3Length(vLook).m128_f32[0];
	m_fDistance = min(4.8f, max(3.6f, m_fDistance * 0.4f));

	vPlayerPosition = XMVectorSetW(vPlayerPosition, 1.f);

	PlayerToMonsterMatrix.r[3] = vPlayerPosition;

	PlayerToMonsterMatrix = SMath::Go_Straight(PlayerToMonsterMatrix, -m_fDistance);
	PlayerToMonsterMatrix = SMath::Go_Up(PlayerToMonsterMatrix, m_fDistance* 0.7f);
	//포지션 추출
	XMStoreFloat3(&m_vTargetPosition, PlayerToMonsterMatrix.r[3]);


	_vector vLookAtPosition = vMonsterPosition;
	vLookAtPosition.m128_f32[1] += m_pLookTargetMonster.lock()->Get_CamOffset();
	//_vector vLookAtPosition = (vMonsterPosition + vPlayerPosition) * 0.5f;
	vLookAtPosition.m128_f32[3] = 1.f;

	PlayerToMonsterMatrix = SMath::LookAt(PlayerToMonsterMatrix, vLookAtPosition);
	

	//로테이션 추출
	XMStoreFloat4(&m_vTargetQuaternion, XMQuaternionRotationMatrix(SMath::Get_RotationMatrix(PlayerToMonsterMatrix)));
}

void CCamera_Target::Bake_TargetJustPlayer(_float fTimeDelta)
{
	
	_vector PlayerPosition = m_pCurrentPlayerTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	PlayerPosition.m128_f32[1] += 2.f;

	_long		MouseMove = 0;

	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
	{
		m_fYAxisRadian += (_float)MouseMove * 0.003f;

	}

	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
	{
		m_fXAxisRadian += (_float)MouseMove * 0.003f;
	}

	_vector TargetQuaternion = XMQuaternionRotationRollPitchYaw(m_fXAxisRadian, m_fYAxisRadian, 0.f);
	_matrix RotationMatrix = XMMatrixRotationQuaternion(TargetQuaternion);
	m_fDistance = min(4.8f, max(3.6f, m_fDistance * 0.4f));
	RotationMatrix = SMath::Go_Straight(RotationMatrix, -m_fDistance);


	RotationMatrix.r[3] += PlayerPosition;
	RotationMatrix.r[3].m128_f32[3] = 1.f;

	XMStoreFloat4(&m_vTargetQuaternion, TargetQuaternion);
	XMStoreFloat3(&m_vTargetPosition, RotationMatrix.r[3]);
	
}

void CCamera_Target::Tick_Shaking(_float fTimeDelta)
{
	if (m_fMaxShakeTime < 0.f)
	{
		m_eCurrentShakeDir = SHAKE_DIRECTION::DIRECTION_END;
		return;
	}

	m_fShakeRatio = m_fCurrentShakeTime / m_fLoopShakeTime;
	m_fShakeRatio = min(1.f, max(0.f, m_fShakeRatio));

	if (m_bReverseShake)
		m_fCurrentShakeTime -= fTimeDelta;
	else
		m_fCurrentShakeTime += fTimeDelta;

	if (m_fCurrentShakeTime >= m_fLoopShakeTime)
		m_bReverseShake = true;
	else if (m_fCurrentShakeTime <= 0.f)
		m_bReverseShake = false;


	m_fMaxShakeTime -= fTimeDelta;
}

void CCamera_Target::Interpolate_Camera(_float fTimeDelta)
{
	_float fLerpRatio = m_isMonsterTarget ? 0.09f : 1.f;

	if (!m_isMonsterTarget && !m_bFirst)
	{
		fLerpRatio = m_fTargetPlayerCamLerpTime;
		m_fTargetPlayerCamLerpTime += fTimeDelta;

		fLerpRatio = min(1.f, fLerpRatio);
	}
	else if (!m_isMonsterTarget && m_bFirst)
	{
		fLerpRatio = 1.f;
		m_bFirst = false;
	}

	m_fCurFreqTime = 0.f;

	_vector vMyQuaternion = XMQuaternionRotationMatrix(SMath::Get_RotationMatrix(m_pTransformCom.lock()->Get_WorldMatrix()));
	_vector vTargetQuaternion = XMLoadFloat4(&m_vTargetQuaternion);
	
	

	_vector vLerpQuaternion = XMQuaternionSlerp(vMyQuaternion, vTargetQuaternion, fLerpRatio);
	m_pTransformCom.lock()->Rotation_Quaternion(vLerpQuaternion);


	_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = XMLoadFloat3(&m_vTargetPosition);
	vTargetPosition.m128_f32[3] = 1.f;

	

	_vector vLerpPosition = XMVectorLerp(vMyPosition, vTargetPosition, fLerpRatio);

	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, vLerpPosition);


	Interpolate_Shaking(fTimeDelta);

	_matrix MyWorldMat = m_pTransformCom.lock()->Get_WorldMatrix();
	MyWorldMat.r[3] += XMLoadFloat3(&m_vShakePosition);
	MyWorldMat.r[3].m128_f32[3] = 1.f;
	//cout << "ShakePosition: " << MyWorldMat.r[3].m128_f32[0] << ", " << MyWorldMat.r[3].m128_f32[1] << ", " << MyWorldMat.r[3].m128_f32[2] << endl;
	//cout << "ShakePosition: " << m_vShakePosition.x << ", " << m_vShakePosition.y << ", " << m_vShakePosition.z << endl;
	XMStoreFloat4x4(&m_MatResult, MyWorldMat);

}

void CCamera_Target::StaticMove_Camera(_float fTimeDelta)
{


}

void CCamera_Target::Interpolate_Shaking(_float fTimeDelta)
{
	//현재 위치 값 소거
	_matrix CameraMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
	CameraMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//최대 목표 거리와 리버스 목표 거리
	_vector vMaxPosition, vMinPosition;

	if (SHAKE_DIRECTION::DIRECTION_END == m_eCurrentShakeDir)
	{
		m_vShakePosition = _float3(0.f, 0.f, 0.f);
	}
	else if (SHAKE_DIRECTION::RIGHT == m_eCurrentShakeDir)
	{
		CameraMatrix = SMath::Go_Right(CameraMatrix, m_fShakePower);
		vMaxPosition = CameraMatrix.r[3];

		CameraMatrix = SMath::Go_Right(CameraMatrix, -m_fShakePower * 2.f);
		vMinPosition = CameraMatrix.r[3];

		XMStoreFloat3(&m_vShakePosition, XMVectorLerp(vMinPosition, vMaxPosition, m_fShakeRatio));
	}
	else if (SHAKE_DIRECTION::UP == m_eCurrentShakeDir)
	{
		CameraMatrix = SMath::Go_Up(CameraMatrix, m_fShakePower);
		vMaxPosition = CameraMatrix.r[3];

		CameraMatrix = SMath::Go_Up(CameraMatrix, -m_fShakePower * 2.f);
		vMinPosition = CameraMatrix.r[3];

		XMStoreFloat3(&m_vShakePosition, XMVectorLerp(vMinPosition, vMaxPosition, m_fShakeRatio));
	}
	else if (SHAKE_DIRECTION::LOOK == m_eCurrentShakeDir)
	{
		CameraMatrix = SMath::Go_Straight(CameraMatrix, m_fShakePower);
		vMaxPosition = CameraMatrix.r[3];

		CameraMatrix = SMath::Go_Straight(CameraMatrix, -m_fShakePower * 2.f);
		vMinPosition = CameraMatrix.r[3];

		XMStoreFloat3(&m_vShakePosition, XMVectorLerp(vMinPosition, vMaxPosition, m_fShakeRatio));
	}
	else
	{
		DEBUG_ASSERT;
	}


}


void CCamera_Target::Update_Cinema(_float fTimeDelta)
{
	m_fCurrentTime += fTimeDelta;

	if (m_fCurrentTime >= (*m_pCurrentCinemaKeys)[m_iCurrentKeyIndex].fTime)
	{

		++m_iCurrentKeyIndex;

		m_fCurrentTime = 0.f;

		if ((*m_pCurrentCinemaKeys).size() <= m_iCurrentKeyIndex)
		{
			m_iCurrentKeyIndex = 0;
			m_bCinema = false;
			return;
		}
	}

	_float fRatio = m_fCurrentTime / (*m_pCurrentCinemaKeys)[m_iCurrentKeyIndex].fTime;

	switch ((*m_pCurrentCinemaKeys)[m_iCurrentKeyIndex].eInterpolationType)
	{
	case INTERPOLATION::LINEAR:
		fRatio;
		break;

	case INTERPOLATION::SQRT:
		fRatio = sqrt(fRatio);
		break;

	case INTERPOLATION::POW:
		fRatio = pow(fRatio, 1.5f);
		break;
	}


	_float3 vPrePitchYawRoll = (*m_pCurrentCinemaKeys)[m_iCurrentKeyIndex - 1].vPitchYawRoll;
	_vector vPrePitchYawRollFromVector = XMLoadFloat3(&vPrePitchYawRoll);
	_vector vPreQuaternion = XMQuaternionRotationRollPitchYawFromVector(vPrePitchYawRollFromVector);
	_float3 vPitchYawRoll = (*m_pCurrentCinemaKeys)[m_iCurrentKeyIndex].vPitchYawRoll;
	_vector vPitchYawRollFromVector = XMLoadFloat3(&vPitchYawRoll);
	_vector vQuaternion = XMQuaternionRotationRollPitchYawFromVector(vPitchYawRollFromVector);
	_vector vCurrentQuaternion = XMQuaternionSlerp(vPreQuaternion, vQuaternion, fRatio);

	_float	fPreDistance = (*m_pCurrentCinemaKeys)[m_iCurrentKeyIndex - 1].fDistance;
	_float	fDistance = (*m_pCurrentCinemaKeys)[m_iCurrentKeyIndex].fDistance;
	_float	fCurrentDistance = SMath::Lerp(fPreDistance, fDistance, fRatio);

	_float3 vPreOffset = (*m_pCurrentCinemaKeys)[m_iCurrentKeyIndex - 1].vOffset;
	_vector vPreOffsetFromVector = XMLoadFloat3(&vPreOffset);
	_float3 vOffset = (*m_pCurrentCinemaKeys)[m_iCurrentKeyIndex].vOffset;
	_vector vOffsetFromVector = XMLoadFloat3(&vOffset);
	_vector vCurrentOffset = XMVectorLerp(vPreOffsetFromVector, vOffsetFromVector, fRatio);
	

	_matrix CurrentMatrix = XMMatrixRotationQuaternion(vCurrentQuaternion);
	CurrentMatrix = SMath::Go_Straight(CurrentMatrix, -fCurrentDistance);

	_matrix PlayerMatrix = m_pCinemaTransformCom.lock()->Get_WorldMatrix();
	PlayerMatrix.r[3] += vCurrentOffset;

	_matrix ResultMatrix = CurrentMatrix * PlayerMatrix;

	XMStoreFloat4x4(&m_MatResult, ResultMatrix);

}

void CCamera_Target::Cinema_LuxiyaUltimate()
{
	m_iCurrentKeyIndex = 0;
	m_fCurrentTime = 0.f;
	m_pCurrentCinemaKeys = &m_tLuxiyaUltimateKeys;
	m_pCinemaTransformCom = m_pCurrentPlayerTransformCom;
	m_bCinema = true;
}

void CCamera_Target::Cinema_BattleEnd()
{
	m_iCurrentKeyIndex = 0;
	m_fCurrentTime = 0.f;
	m_pCurrentCinemaKeys = &m_tBattleEndCinemaKeys;
	m_pCinemaTransformCom = m_pCurrentPlayerTransformCom;
	m_bCinema = true;

}

void CCamera_Target::Cinema_LuxiyaStartBattle()
{
	m_iCurrentKeyIndex = 0;
	m_fCurrentTime = 0.f;
	m_pCurrentCinemaKeys = &m_tLuxiyaBornKeys;
	m_pCinemaTransformCom = m_pCurrentPlayerTransformCom;
	m_bCinema = true;

}

void CCamera_Target::Cinema_BossStartBattle()
{
	m_iCurrentKeyIndex = 0;
	m_fCurrentTime = 0.f;
	m_pCurrentCinemaKeys = &m_tBossStartCinemaKeys;
	m_bCinema = true;
}

void CCamera_Target::OnLevelExit()
{
	m_bCinema = false;
	m_bFirst = true;
}

void CCamera_Target::Free()
{
}
