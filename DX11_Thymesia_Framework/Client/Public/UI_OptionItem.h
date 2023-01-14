#pragma once
#include "UI_Button.h"

BEGIN(Client)

class CCustomUI;
class CUI_OptionElementBase;

class CUI_OptionItem :  public CUI_Button
{
public:
	GAMECLASS_H(CUI_OptionItem)
	CLONE_H(CUI_OptionItem , CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	//For Scroll
	virtual void	Set_OriginCenterPosFromThisPos();
	void			Set_ScrollOffsetY(_float fOffsetY);
	void			Add_ScrollOffsetY(_float fOffsetY);
	void			ResetPos();


public:
	void			Create_Element(OPTION_TYPE eOptionType);

public:
	//Å¸ÀÔ, °ª
	FDelegate<OPTION_TYPE, _uint> Callback_OnChangeOption;
	void			Call_OnChangeOption(_int iAmount);


protected:
	virtual void	OnMouseOver() override;
	virtual void	OnMouseOut() override;

private:
	weak_ptr<CCustomUI>			m_pOptionName;
	_int						m_iAmount = 0;


	weak_ptr<CUI_OptionElementBase> m_pElement;


private:
	OPTION_TYPE				m_eOptionType;
	_float					m_fScrollOffsetY = 0.f;
	_float2					m_fOriginCenterPos = { 0.f,0.f };
};

END
