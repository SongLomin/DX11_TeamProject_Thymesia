#pragma once

#include "Client_Defines.h"
#include "ClientLevel.h"

BEGIN(Client)

class CImGui_Manager;

class CLevel_Edit final : public CClientLevel
{
	GAMECLASS_H(CLevel_Edit);

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	static shared_ptr<CLevel_Edit> Create();
	virtual void Free() override;

private:
	unique_ptr<CImGui_Manager> m_pImGui_Manager;
};

END