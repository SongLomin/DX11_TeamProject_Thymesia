#pragma once
#include "Camera.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Static final : public CCamera
{
	GAMECLASS_H(CCamera_Static);
	CLONE_H(CCamera_Static, CGameObject);

public:
	CCamera_Static(const CCamera_Static& rhs);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;


public:
	void Free();
};

END