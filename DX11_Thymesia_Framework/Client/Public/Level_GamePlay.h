#pragma once

#include "Client_Defines.h"
#include "ClientLevel.h"


BEGIN(Client)

class CLevel_GamePlay final : public CClientLevel
{
public:
	CLevel_GamePlay();
	virtual ~CLevel_GamePlay();

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();
	

public:
	static shared_ptr<CLevel_GamePlay> Create();

private:
	_bool	m_bCheckMonster = false;
	_int	m_iWaveCount = 3;

private:
	virtual void OnEventMessage(_uint iArg) override;
	void Free();
};

END