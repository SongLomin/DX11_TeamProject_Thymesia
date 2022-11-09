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

class CCamera_Target final : public CCamera
{
	GAMECLASS_H(CCamera_Target);
	CLONE_H(CCamera_Target, CGameObject);

	enum class CAMERA_STATE
	{
		LOOKMONSTER,
		INTERPOLATE,
		LOOKPLAYER,
		STATE_END
	};

public:


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

	void Start_Cinematic(weak_ptr<CModel> _pModel,const _char* pBoneName);
	void End_Cinematic();
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

	void Interpolate_Camera(_float fTimeDelta);

	void Update_Bone();


private:
	_bool					m_bCinematic = false;
	weak_ptr<CTransform>	m_pCameraBoneParentTransform;
	weak_ptr<CBoneNode>		m_pCameraBoneNode;
	_float4x4				m_TransformationMatrix;
	_float4x4				m_OriginalMatrix;

	weak_ptr<CPlayer>		m_pCurrentPlayer;
	weak_ptr<CTransform>	m_pCurrentPlayerTransformCom;

	weak_ptr<CGameObject>		m_pTargetMonster;
	weak_ptr<CTransform>	m_pTargetMonsterTransformCom;

	CAMERA_STATE			m_eCameraState = CAMERA_STATE::STATE_END;
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

	

private:
	virtual void OnLevelExit() override;
	void Free();
};

END