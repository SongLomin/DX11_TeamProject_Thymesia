#pragma once
#include "UI_Button.h"

BEGIN(Client)

class CCustomUI;
class CEasingComponent_Alpha;

class CUI_InventoryConfirmWindowButton : public CUI_Button
{	
public:
	GAMECLASS_H(CUI_InventoryConfirmWindowButton)
	CLONE_H(CUI_InventoryConfirmWindowButton,CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);


private:
	virtual void	Create_UI();


	virtual void SetUp_Component() override;

private:
	weak_ptr<CCustomUI>		m_pFontImage;
	weak_ptr<CCustomUI>		m_pBG;

private:
	weak_ptr<CEasingComponent_Alpha>	m_pEasingAlpha;
private:
	void		Free() {};

};

END
