#pragma once

#include "Client_Defines.h"
#include "ClientLevel.h"

BEGIN(Client)
class CPlayer;

class CLevel_Stage2 final : public CClientLevel
{
public:
	CLevel_Stage2();
	virtual ~CLevel_Stage2();

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	virtual void ExitLevel(LEVEL eLevel);

private:
	weak_ptr<CPlayer> m_pPlayer;

public:
	static shared_ptr<CLevel_Stage2> Create();
	void Free();
};

END