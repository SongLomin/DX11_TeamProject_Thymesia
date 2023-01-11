#pragma once
#include "CustomUI.h"

BEGIN(Client)

class CEasingComponent_Alpha;

class CUI_FadeMask : public CCustomUI
{
public:
	GAMECLASS_H(CUI_FadeMask)
	CLONE_H(CUI_FadeMask, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

private:


private:
	void			Free();

};


END

