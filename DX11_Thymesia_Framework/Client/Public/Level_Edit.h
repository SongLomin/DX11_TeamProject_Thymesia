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
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	static shared_ptr<CLevel_Edit> Create();
	void Free();

private:
	shared_ptr<CImGui_Manager> m_pImGui_Manager;
#ifdef _RENDER_FPS_
	_float	m_fTimeAcc = 0.f;
	_uint	m_iNumRender = 0;
	_tchar	m_szFPS[MAX_PATH] = TEXT("");
	TEXTINFO m_TextInfo_FPS;
#endif // _RENDER_FPS_
};

END