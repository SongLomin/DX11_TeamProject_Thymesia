#pragma once
#include "UI.h"

BEGIN(Client)

class CProgressBar;

class CPlayer_MPBar final : public CUI
{
	GAMECLASS_H(CPlayer_MPBar);
	CLONE_H(CPlayer_MPBar, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void OnEventMessage(_uint iArg) override;

public:
	void		Set_CurrentMp(_float	_fCurrentMp);
	void		Set_MaxMp(_float	_fMaxMp) { m_fMaxMp = _fMaxMp; }

protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:
	weak_ptr< CProgressBar>	m_pMainBar;
	weak_ptr< CProgressBar>	m_pBG;
	weak_ptr< CProgressBar>	m_pBorderLeft;
	weak_ptr< CProgressBar>	m_pBorderRight;

private:
	_float			m_fLerpAcc;

private:
	_float			m_fMaxMp;
	_float			m_fCurrentMp;
	_float			m_fLerpMp;


private:
	TEXTINFO		m_tTextInfo;

public:
	void Free();

};

END

