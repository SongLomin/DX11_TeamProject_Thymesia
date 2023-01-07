#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Interior final : public CCamera
{
	GAMECLASS_H(CCamera_Interior)
	CLONE_H(CCamera_Interior, CGameObject)

public:
	CCamera_Interior(const CCamera_Interior& rhs);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
	virtual void  OnEventMessage(_uint iArg) override;

private:
	const void ControlCamera(const _float& fTimeDelta);
	const void MoveCamera_KeyInput(const _float& fTimeDelta);
	const void MoveCamera_MouseInput(const _float& fTimeDelta);

private:
	_int m_iCameraSpeed = 0;

public:
	void Free();
};

END