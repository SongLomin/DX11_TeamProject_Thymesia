#pragma once
#include "UI.h"

BEGIN(Client)

class CCustomUI;
class CPlayer_ProgressBar;
class CHUD_Hover;

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

	virtual HRESULT Render() { return S_OK; }

protected:
	virtual HRESULT SetUp_ShaderResource() { return S_OK; }

private:
	weak_ptr<CCustomUI>		m_pLoadingBG;
	weak_ptr<CHUD_Hover>	m_pIcon;

private:
	_float					m_fRatio;

public:
	void Free();


};

END