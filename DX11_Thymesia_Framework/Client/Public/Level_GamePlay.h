#pragma once
#include "Client_Defines.h"
#include "ClientLevel.h"

BEGIN(Client)
class CUI_PauseMenu;
class CPlayer;

class CLevel_GamePlay final : public CClientLevel
{
public:
	CLevel_GamePlay();
	virtual ~CLevel_GamePlay();

	virtual HRESULT Initialize();
	virtual void	Tick(_float TimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

	static shared_ptr<CLevel_GamePlay> Create();

	virtual void ExitLevel(LEVEL eLevel);

private:
	virtual void OnEventMessage(_uint iArg) override;
	void Free();

private:
	weak_ptr<CPlayer> m_pPlayer;

private:
	_bool m_bCheckMonster = false;
	_int  m_iWaveCount    = 3;
};
END
