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
	virtual HRESULT Render();

private:	
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

public:
	static shared_ptr<CLevel_Logo> Create();
	virtual void Free() override;
};

END