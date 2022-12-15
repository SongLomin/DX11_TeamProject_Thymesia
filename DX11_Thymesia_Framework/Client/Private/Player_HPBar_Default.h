#pragma once
#include "UI.h"


BEGIN(Client)

class CPlayer_HPBar_Default final : public CUI
{
	GAMECLASS_H(CPlayer_HPBar_Default);
	CLONE_H(CPlayer_HPBar_Default, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	virtual void OnEventMessage(_uint iArg) override;

public:

public:

protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:

private:
	void Free();

};

END

