#pragma once
#include "UI.h"
#include "UI_LerpBar.h"

BEGIN(Client)

class CProgressBar;

class CPlayer_MPBar final : public CUI_LerpBar
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

	void		Set_CurrentHp(_float	_fCurrentHp, _bool bLerp = false,
		EASING_TYPE eLerpType = EASING_TYPE::QUAD_IN);
	void		Set_MaxMp(_float	_fMaxMp) { m_fMaxMp = _fMaxMp; }

protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:
	weak_ptr< CProgressBar>	m_pMainBar;
	weak_ptr< CProgressBar>	m_pBG;
	weak_ptr< CProgressBar>	m_pBorderLeft;
	weak_ptr< CProgressBar>	m_pBorderRight;

private:
	_float			m_fMaxMp;
	_float			m_fLerpedMp;
	_float			m_fCurrentMp;


private:
	TEXTINFO		m_tTextInfo;

public:
	void Free();

};

END

