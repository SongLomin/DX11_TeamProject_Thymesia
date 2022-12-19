#pragma once
#include "CustomUI.h"
#include "UI_Button.h"

BEGIN(Client)

class CustomUI;
class CItem;

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
	virtual void	Set_OriginCenterPosFromThisPos();

	void			Set_ScroolOffsetY(_float fOffsetY);
	void			Add_ScroolOffsetY(_float fOffsetY);
	void			ResetPos();
	
	void			Bind_Item(weak_ptr<CItem> pItem);
	_bool			Is_Bind();
	void			UnBind_Item();
public:
	virtual void	OnMouseOver() override;
	virtual void	OnMouseOut() override;



public:
	FDelegate<weak_ptr<CItem>>	Callback_OnMouseOver;
	FDelegate<>	Callback_OnMouseOut;


private:
	void			Create_ItemSlot();
	void			Create_TextInfo();

private:
	_bool			Check_IsInInventoryFrame();
	void			Update_TextInfo();
//private아이템 바인딩 해줘야함.


private:
	_float					m_fScrollOffsetY = 0.f;
	_float2					m_fOriginCenterPos = { 0.f,0.f };
private:
	weak_ptr<CCustomUI>		m_pMain;
	weak_ptr<CCustomUI>		m_pFrame;
	weak_ptr<CCustomUI>		m_pHover;

	weak_ptr<CCustomUI>		m_pIcon;
	weak_ptr<CItem>			m_pBindedItem;


	TEXTINFO				m_tTextInfo;

};


END