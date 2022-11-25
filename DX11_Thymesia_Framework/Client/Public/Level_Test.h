#pragma once
#include "Client_Defines.h"
#include "ClientLevel.h"


BEGIN(Client)

class CUI_PauseMenu;

class CLevel_Test final : public CClientLevel
{
public:
	CLevel_Test();
	virtual ~CLevel_Test();

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	static shared_ptr<CLevel_Test> Create();

private:
	_bool	m_bCheckMonster = false;
	_int	m_iWaveCount = 3;
	_bool	m_bFadeTrigger = false;


private:
	virtual void OnEventMessage(_uint iArg) override;
	void Free();



};

END