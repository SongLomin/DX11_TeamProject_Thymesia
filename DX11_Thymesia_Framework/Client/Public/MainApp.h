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

class CMainApp final : public CBase
{
public:
	CMainApp();
	virtual ~CMainApp();
public:
	HRESULT Initialize();	
	void Tick(float fTimeDelta);
	HRESULT Render();

private:
	_float	m_fTimeAcc = 0.f;
	_uint	m_iNumRender = 0;
	_tchar	m_szFPS[MAX_PATH] = TEXT("");


private:
	/* 그리기 상태를 셋팅하낟. */
	//HRESULT SetUp_RenderState();
	//HRESULT SetUp_SamplerState();

	HRESULT Open_Level(LEVEL eLevelID);
	HRESULT Ready_Prototype_Component();


	

public:
	static unique_ptr<CMainApp> Create();
	virtual void Free();

};

END