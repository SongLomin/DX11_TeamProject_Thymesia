#pragma once
#include "CustomUI.h"
#include "UI_Button.h"

BEGIN(Client)

class CustomUI;

class CUI_ItemSlot : public CUI_Button_Base
{
public:
	GAMECLASS_H(CUI_ItemSlot)
	CLONE_H(CUI_ItemSlot, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);


public:
	virtual void    Set_UIPosition(const _float fX, const _float fY, UI_ALIGN_TYPE eType = UI_ALIGN_TYPE::ALIGN_CENTER) override;

private:
	void			Create_ItemSlot();
	void			Create_TextInfo();

private:
	_bool			Check_IsInInventoryFrame();

//private아이템 바인딩 해줘야함.


private:
	weak_ptr<CCustomUI>		m_pMain;
	weak_ptr<CCustomUI>		m_pFrame;
	weak_ptr<CCustomUI>		m_pHover;
	TEXTINFO				m_tTextInfo;

};


END