#pragma once
#include "UI.h"

BEGIN(Client)

class CCustomUI;
class CProgressBar;
class CHUD_Hover;
class CDissolveUI;
class CLoadingTextUI;
class CUI_Loading : public CUI
{
public:
	GAMECLASS_H(CUI_Loading)
	CLONE_H(CUI_Loading, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) { return S_OK; }

	void Call_FadeEnd();

public:
	void			SetUp_LoadingUI(LEVEL eLevel);

public:
	void			Set_Complete();
	_bool			Get_Finish();

protected:
	virtual HRESULT SetUp_ShaderResource() { return S_OK; }

private:
	weak_ptr<CCustomUI>		m_pLoadingBG;
	weak_ptr<CHUD_Hover>	m_pIcon;
	weak_ptr<CHUD_Hover>	m_pLoadComplete;

	weak_ptr<CLoadingTextUI>		m_pStageTitle;
	weak_ptr<CLoadingTextUI>		m_pStageDesc;


private:
	_bool					m_bLoadComplete;
	_bool					m_bCallFadeOut;
	_bool					m_bIsChangeLevel;
public:
	void Free();

};

END