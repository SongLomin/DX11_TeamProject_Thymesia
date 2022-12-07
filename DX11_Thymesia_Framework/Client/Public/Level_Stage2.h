#pragma once

#include "Client_Defines.h"
#include "ClientLevel.h"

BEGIN(Client)

class CLevel_Stage2 final : public CClientLevel
{
public:
	CLevel_Stage2();
	virtual ~CLevel_Stage2();

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	static shared_ptr<CLevel_Stage2> Create();
	void Free();
private:
	_bool m_bFadeTrigger = false;

};

END