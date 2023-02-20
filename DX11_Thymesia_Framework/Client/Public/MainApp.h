#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;

END

BEGIN(Client)
//class CGameInstance;

class CImGui_Manager;
class CDeveloperConsole_Manager;

class CMainApp final
	: public CBase
{
public:
	CMainApp();
	virtual ~CMainApp();

	HRESULT Initialize();	
	void Tick(float fTimeDelta);
	HRESULT Render();

private:
#ifdef _RENDER_FPS_
	_float	m_fTimeAcc = 0.f;
	_uint	m_iNumRender = 0;
	_tchar	m_szFPS[MAX_PATH] = TEXT("");
	TEXTINFO m_TextInfo_FPS;
#endif // _RENDER_FPS_

	_bool	m_bClip = true;

	shared_ptr<CDeveloperConsole_Manager>	m_pDeveloperConsole;
	_bool									m_bEnableConsole = false;

private:
	/* 그리기 상태를 셋팅하낟. */
	//HRESULT SetUp_RenderState();
	//HRESULT SetUp_SamplerState();
	HRESULT Open_Level(LEVEL eLevelID);

public:
	static void	Bake_MipMaps();
	static void	Bake_MipMaps_Recursive(const filesystem::path& In_Path);


public:
	static unique_ptr<CMainApp> Create();

private:
	void Free();

};

END