#pragma once
#include "UI.h"
#include "UI_Container.h"
BEGIN(Client)

class CUI_Button_Base abstract : public CUI_Container
{
public:
	enum UI_BUTTON_STATE
	{
		UI_BUTTON_OVER,
		UI_BUTTON_BUTTON_DOWN,
		UI_BUTTON_NONE
	};

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	_bool		Get_Togle() { return m_bClickToggle; }

protected:
	 void	Update_MouseOver();
	 void	Update_LButtonDown();
	 void	Update_LClick();
public:
	FDelegate<>		Callback_OnLButtonClicked;
	FDelegate<>		Callback_OnMouseOver;
	FDelegate<>		Callback_OnMouseOut;
	FDelegate<>		Callback_OnLButtonDown;

protected:
	virtual void	OnMouseOver();
	virtual void	OnMouseOut();
	virtual void	OnLButtonDown();
	virtual void	OnLButtonClick();




private:
	UI_BUTTON_STATE m_eButtonState;
	_bool		m_bClickToggle;
};

END
