#pragma once
#include "CustomUI.h"

BEGIN(Client)

class CEasingComponent_Alpha;
class CEasingComponent_Transform;


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
	weak_ptr<CEasingComponent_Alpha>		m_pEasingSize;
	weak_ptr<CEasingComponent_Alpha>		m_pEasingAlpha;

public:
	void									Set_Fade(_float fStart, _float fTarget, _float fTime, EASING_TYPE eType);
	

public:
	FDelegate<>								Callback_OnLerpEnd;
	void									Call_OnLerpEnd();

	FDelegate<void*>						Callback_OnLerpEndSetEnable;
	void									Call_OnLerpEndEnable();

	FDelegate<>								Callback_OnLerpEndSetDisable;
	void									Call_OnLerpEndDisable();



private:
	void			Free();

};


END

