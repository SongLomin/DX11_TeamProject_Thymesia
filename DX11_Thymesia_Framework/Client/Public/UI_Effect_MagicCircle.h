#pragma once
#include "UI_EffectBase.h"

BEGIN(Client)

class CUI_Effect_MagicCircle : public CUI_EffectBase
{
public:
	GAMECLASS_H(CUI_Effect_MagicCircle)
	CLONE_H(CUI_Effect_MagicCircle, CGameObject)


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);


private:

};

END

