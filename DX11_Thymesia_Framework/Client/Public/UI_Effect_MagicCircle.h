#pragma once
#include "UI_EffectBase.h"

BEGIN(Client)

class CUI_Effect_MagicCircle : public CUI_EffectBase
{
public:
	GAMECLASS_H(CUI_Effect_MagicCircle)
	CLONE_H(CUI_Effect_MagicCircle, CGameObject)

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;


public:
	void		Play(_float2 fPt, _float fSizeMag);
	
private:
	virtual void Init_UI() override;

private:
	_float		m_fPlayTime = 0.f;

};

END

