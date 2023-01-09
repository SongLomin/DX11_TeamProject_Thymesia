#pragma once
#include "Client_Defines.h"
#include "ClientLevel.h"

BEGIN(Client)
class CUI_PauseMenu;

class CLevel_Home final
	: public CClientLevel
{
public:
	CLevel_Home();
	virtual ~CLevel_Home();

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

	static shared_ptr<CLevel_Home> Create();

	virtual void ExitLevel(LEVEL eLevel);

private:
	virtual void OnEventMessage(_uint iArg) override;
	void Free();

private:
	_bool	m_bCheckMonster = false;
	_int	m_iWaveCount    = 3;
	_bool	m_bFadeTrigger  = false;
};

END
