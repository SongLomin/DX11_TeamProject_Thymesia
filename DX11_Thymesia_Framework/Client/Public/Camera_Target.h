#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CTransform;
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
	void Focus_Monster(weak_ptr<CMonster> _pMonster);
	void Release_Focus();
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
	void Bake_TargetCamera(_float fTimeDelta);

	void Interpolate_Camera(_float fTimeDelta);



private:
	weak_ptr<CPlayer>		m_pCurrentPlayer;
	weak_ptr<CTransform>	m_pCurrentPlayerTransformCom;

	weak_ptr<CMonster>		m_pTargetMonster;
	weak_ptr<CTransform>	m_pTargetMonsterTransformCom;

	_float3					m_vTargetPosition;
	_float4					m_vTargetQuaternion;
	
	_float4x4				m_MatResult;

	_float					m_fDistance = 0.f;

	CAMERA_STATE			m_eCameraState = CAMERA_STATE::STATE_END;
	_bool					m_bIsFocused = false;
	_bool					m_bFirst = true;		
	

private:
	virtual void OnLevelExit() override;
	void Free();
};

END