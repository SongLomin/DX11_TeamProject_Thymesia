#pragma once
#include "Client_Defines.h"
#include "ClientLevel.h"

BEGIN(Client)

class CLevel_Lobby final : public CClientLevel
{
	GAMECLASS_H(CLevel_Lobby);

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();
	

public:
	static shared_ptr<CLevel_Lobby> Create();
	void Free();
};

END