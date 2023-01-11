#pragma once
#include "UI_Button.h"


BEGIN(Client)

class CUI_InventoryConfirmWindow : public CUI_Button
{
	GAMECLASS_H(CUI_InventoryConfirmWindow)
	CLONE_H(CUI_InventoryConfirmWindow, CGameObject)
public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

private:

};

END
