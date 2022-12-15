#pragma once

#include "Client_Defines.h"
#include "ClientLevel.h"

BEGIN(Client)

class CFadeMask;

class CLevel_Logo final : public CClientLevel
{
public:
	CLevel_Logo();
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	 void ExitLevel(LEVEL eLevel);
public:
	static shared_ptr<CLevel_Logo> Create();
	void Free();

private:
	virtual void		SetUp_UI() override;

};

END