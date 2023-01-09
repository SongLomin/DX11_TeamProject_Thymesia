#pragma once
#include "UI_EffectBase.h"

BEGIN(Client)

class CUI_CustomEffect : public CUI_EffectBase
{
public:
	GAMECLASS_H(CUI_CustomEffect)
	CLONE_H(CUI_CustomEffect, CGameObject)

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	void	Set_LerpAlpha(_float fStart, _float fTarget, _float fTime, EASING_TYPE eType);
	void	Set_LerpSize(_float2 fStart, _float2 fTarget, _float fTime, EASING_TYPE eType);
	void	Set_LerpTransform(_float2 fStart, _float2 fTarget, _float fTime, EASING_TYPE eType);

private:
	_float		m_fPlayTime = 0.f;

private:
	void		Free();

};

END

