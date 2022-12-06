#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CTransform;
class CModel;
class CBoneNode;
END

BEGIN(Client)
class CPlayer;
class CMonster;
class CPhysXCameraController;

class CCamera_Target final : public CCamera
{
	GAMECLASS_H(CCamera_Target);
	CLONE_H(CCamera_Target, CGameObject);

public:
	CCamera_Target(const CCamera_Target& rhs);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Target();
	void Focus_Monster(weak_ptr<CGameObject> _pMonster);
	void Release_Focus();

	void Start_Cinematic(weak_ptr<CModel> _pModel,const _char* pBoneName, _fmatrix OffSetMatrix, CINEMATIC_TYPE eType);
	void End_Cinematic();

	void Activate_Zoom(_float fRatio, _float fZoomTime);
	void Deactivate_Zoom();

	void Add_Shaking(_vector vShakingDir, _float fRatio, _float fShakingTime, _float fFrequency);
	/*
	* 플레이어한테도 해당 타겟을 넘겨줌 <-선형 보간으로 플레이어 방향을 몬스터를 향하게 도렬둠
	* 게임 매니저에서 몬스터의포인터를 가지고 있음
	* 게임 매니저에서 몬스터 정렬하고 카메라 절두체 내부에 있는 몬스터들 추려냄 범위 내에 있는지 플레이어 거리 순으로 오름차순 정렬 그 중 가장 가까운애를 
	* 게임매니저에서 Focus_Monster를 부르면 카메라의 focus_Monster,플레이어의 focus_Monster를 호출 각각 한번씩만 해줌
	* 한번 타겟을 찾으면 바꾸기 전까지 고정이므로 바꿀때마다 갱신
	*/

protected:
	virtual HRESULT Bind_PipeLine() override;

private:
	void Look_At_Target(_float fTimeDelta);
	void Free_MouseMove(_float fTimeDelta);

	void Calculate_ZoomOffSet(_float fTimeDelta);
	void Calculate_ShakingOffSet(_float fTimeDelta);

	void Interpolate_Camera(_float fTimeDelta);
	void Reposition_Camera_AfterCinematic(_float fTimeDelta);

	void Update_Bone();
	void Update_PhysXCollider(_float fTimeDelta);


private:
	weak_ptr<CPhysXCameraController> m_pPhysXCameraControllerCom;

	//연출이 끝난 후에 위치 보정 용
	CINEMATIC_TYPE					m_eCinematicType = CINEMATIC_TYPE::TYPE_END;
	_bool					m_bCinematicEnd = false;
	_float4					m_vDestCamPosition;
	_float4					m_vCamPosAfterCinematic;


	_bool					m_bCinematic = false;
	weak_ptr<CTransform>	m_pCameraBoneParentTransform;
	weak_ptr<CBoneNode>		m_pCameraBoneNode;
	_float4x4				m_TransformationMatrix;
	_float4x4				m_OriginalMatrix;
	_float4x4				m_CinemaWorldMatrix;
	_float4x4				m_CinematicOffSetMatrix;

	weak_ptr<CPlayer>		m_pCurrentPlayer;
	weak_ptr<CTransform>	m_pCurrentPlayerTransformCom;

	weak_ptr<CGameObject>		m_pTargetMonster;
	weak_ptr<CTransform>	m_pTargetMonsterTransformCom;

	_bool					m_bIsFocused = false;
	_bool					m_bFirst = true;

	_long m_iMouseMovementX = 0;
	_long m_iMouseMovementY = 0;

	_float3 m_vOffSet = _float3(0.f, 0.f, 0.f);
	_float4 m_vPrePlayerPos = _float4(0.f, 0.f, 0.f, 0.f);

	_float m_fRotationLerpRatio = 0.f;
	_float m_fAccel = 10.f;
	_float m_fSpeed = 0.f;
	_float4 m_vPlayerFollowLerpPosition = _float4(0.f, 0.f, 0.f, 1.f);

	
	_float m_fZoom = 0.f;//현재 줌 비율 값
	_float m_fZoomStartOffSet = 0.f;//시작 값
	_float m_fZoomEndOffSet = 0.f;//도착 값
	_float m_fZoomTime = 0.f; // 줌 지속 시간
	_float m_fZoomTimeAcc = 0.f;

	_float3 m_vShakingStartOffSet = _float3(0.f, 0.f, 0.f);
	_float3 m_vShaking = _float3(0.f, 0.f, 0.f);
	_float3 m_vShakingEndOffSet = _float3(0.f, 0.f, 0.f);
	_float3 m_vShakingDir = _float3(0.f, 0.f, 0.f);
	_float m_fShakingTime = 0.f;
	_float m_fShakingFrequency = 0.f;
	_float m_fShakeRatio = 0.f;
	_float m_fShakingTimeAcc = 0.f;
	_float m_fShakingQuarterFrequency = 0.f;
	_float m_fShakingDecreaseTime = 0.f;


	_float4x4 m_CollisionMatrix;
	_bool		m_bCollision = false;

private:
	virtual void OnLevelExit() override;
	void Free();
};

END