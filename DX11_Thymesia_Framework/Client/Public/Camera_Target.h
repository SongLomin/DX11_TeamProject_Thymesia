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
	* �÷��̾����׵� �ش� Ÿ���� �Ѱ��� <-���� �������� �÷��̾� ������ ���͸� ���ϰ� ���ĵ�
	* ���� �Ŵ������� �����������͸� ������ ����
	* ���� �Ŵ������� ���� �����ϰ� ī�޶� ����ü ���ο� �ִ� ���͵� �߷��� ���� ���� �ִ��� �÷��̾� �Ÿ� ������ �������� ���� �� �� ���� �����ָ� 
	* ���ӸŴ������� Focus_Monster�� �θ��� ī�޶��� focus_Monster,�÷��̾��� focus_Monster�� ȣ�� ���� �ѹ����� ����
	* �ѹ� Ÿ���� ã���� �ٲٱ� ������ �����̹Ƿ� �ٲܶ����� ����
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

	//������ ���� �Ŀ� ��ġ ���� ��
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

	
	_float m_fZoom = 0.f;//���� �� ���� ��
	_float m_fZoomStartOffSet = 0.f;//���� ��
	_float m_fZoomEndOffSet = 0.f;//���� ��
	_float m_fZoomTime = 0.f; // �� ���� �ð�
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