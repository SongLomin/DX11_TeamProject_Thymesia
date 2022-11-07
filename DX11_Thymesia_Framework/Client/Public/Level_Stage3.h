#pragma once

#include "Client_Defines.h"
#include "ClientLevel.h"

BEGIN(Client)

class CLevel_Stage3 final : public CClientLevel
{
public:
	CLevel_Stage3();
	virtual ~CLevel_Stage3();

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	static shared_ptr<CLevel_Stage3> Create();
	virtual void Free() override;
};

END