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

	virtual HRESULT Initialize();
	virtual void	Tick(_float TimeDelta);
	virtual HRESULT Render();

	static shared_ptr<CLevel_GamePlay> Create();
	void Call_Enable_PauseMenu();

private:
	virtual void OnEventMessage(_uint iArg) override;
	void Free();

private:
	_bool m_bCheckMonster = false;
	_bool m_bFadeTrigger  = false;
	_int  m_iWaveCount    = 3;
};
END
