#pragma once
#include "CustomUI.h"
#include "UI_Button.h"

BEGIN(Client)

class CustomUI;
class CItem;
class CEasingComponent_Transform;


class CUI_ItemSlot : public CUI_Button
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

	void			Set_ScrollOffsetY(_float fOffsetY);
	void			Add_ScrollOffsetY(_float fOffsetY);
	void			ResetPos();
	
	weak_ptr<CItem>	Get_BindItem() { return m_pBindedItem; }

	void			Bind_Item(weak_ptr<CItem> pItem);
	_bool			Is_Bind();
	void			UnBind_Item();


	virtual void	Set_RenderGroup(RENDERGROUP eRenderGroup) override;
	void			Update_TextInfo();
public:
	virtual void	OnMouseOver() override;
	virtual void	OnMouseOut() override;
	void			OffMouseHover();


public:
	virtual void	Lerp_Transform(_float2	vTargetPos, _float fLerpTime);
	virtual void	Set_RenderIcon(_bool bRenderIcon) { m_bRenderIcon = bRenderIcon; }

public:
	FDelegate<weak_ptr<CItem>>	Callback_OnMouseOver;
	FDelegate<>	Callback_OnMouseOut;

private:
	void			Create_ItemSlot();
	void			Create_TextInfo();

private:
	_bool			Check_IsInInventoryFrame();

	virtual void	SetUp_Component() override;
//private아이템 바인딩 해줘야함.


private:
	_float					m_fScrollOffsetY = 0.f;
	_float2					m_fOriginCenterPos = { 0.f,0.f };

private:
	weak_ptr<CEasingComponent_Transform>	m_pEasingTransform;

private:
	weak_ptr<CCustomUI>		m_pMain;
	weak_ptr<CCustomUI>		m_pFrame;
	weak_ptr<CCustomUI>		m_pHover;

	weak_ptr<CCustomUI>		m_pIcon;
	weak_ptr<CItem>			m_pBindedItem;
	_bool					m_bRenderIcon = true;

	TEXTINFO				m_tTextInfo;

};


END