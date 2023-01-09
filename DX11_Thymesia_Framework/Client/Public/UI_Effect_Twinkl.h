#pragma once
#include "UI_EffectBase.h"

BEGIN(Client)

class CUI_Effect_Twinkl: public CUI_EffectBase
{
public:
	GAMECLASS_H(CUI_Effect_Twinkl)
	CLONE_H(CUI_Effect_Twinkl, CGameObject)

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	void			Play(_float2 fMousePt, _float fSizeMag);
	virtual void	Init_UI() override;

private:
	_float		m_fPlayTime = 0.f;

private:
	void		Free();

};

END

