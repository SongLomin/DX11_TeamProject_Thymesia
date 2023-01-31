#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Free final : public CCamera
{
	GAMECLASS_H(CCamera_Free)
	CLONE_H(CCamera_Free, CGameObject)

public:
	CCamera_Free(const CCamera_Free& rhs);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
	virtual void  OnEventMessage(_uint iArg) override;

private:
#ifdef _DEBUG
	const void ReadyText();
	const void UpdateText();
#endif // _DEBUG
	const void ControlCamera
	(
		const _float& fTimeDelta
	);
	const void MoveCamera_KeyInput(const _float& fTimeDelta);
	const void MoveCamera_MouseInput(const _float& fTimeDelta);
	const void ChangeCameraSpeed();

private:
	_int		m_iCameraSpeed		= 0;
	_float		m_fCameraTurnRatio	= 0.1f;
#ifdef _DEBUG
	TEXTINFO m_tCameraSpeedText;
#endif // _DEBUG

public:
	void Free();
};

END