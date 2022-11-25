#pragma once

#include "Client_Defines.h"
#include "ClientLevel.h"


BEGIN(Client)

class CUI_PauseMenu;

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


public:
	void		Call_Enable_PauseMenu();
private:
	_bool	m_bCheckMonster = false;
	_int	m_iWaveCount = 3;
	_bool	m_bFadeTrigger = false;


private:
	virtual void OnEventMessage(_uint iArg) override;
	void Free();



};

END