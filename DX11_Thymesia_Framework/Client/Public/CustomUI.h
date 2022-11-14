#pragma once
#include "UI.h"


BEGIN(Client)

class CCustomUI : public CUI
{
public:
	GAMECLASS_H(CCustomUI);
	CLONE_H(CCustomUI, CGameObject);
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void		Set_PassIndex(_uint _iPassIndex);

	void		Set_AlphaColor(_float fAlphaColor) { m_fAlphaColor = fAlphaColor; }
	_float		Get_AlphaColor() { return m_fAlphaColor; }

protected:
	virtual HRESULT SetUp_ShaderResource() override;

protected:
	_float	m_fAlphaColor = 1.f;

private:

public:
	void Free();
};

END

