#pragma once
#include "UI.h"
#include "UI_Container.h"
BEGIN(Client)

class CUI_Button : public CCustomUI
{
public:
	GAMECLASS_H(CUI_Button)
	CLONE_H(CUI_Button, CGameObject)

public:
	enum UI_BUTTON_STATE
	{
		UI_BUTTON_OVER,
		UI_BUTTON_BUTTON_DOWN,
		UI_BUTTON_OUT
	};

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

	virtual void			Set_ButtonLevel(BUTTON_LEVEL eButtonLevel);
	virtual BUTTON_LEVEL	Get_ButtonLevel() { return m_eButtonLevel; }

public:
	_bool		Get_Togle() { return m_bClickToggle; }
	UI_BUTTON_STATE	Get_ButtonState() { return m_eButtonState; }
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
	virtual void	OnLButtonUp();


	virtual void	OnLButtonClick();
	
protected:
	virtual void	CheckMouseOver();
	virtual void	CheckMouseOut();
	virtual void	CheckLButtonDown();
	virtual void	CheckLButtonClick();

	virtual void	UnCheckMouseOver();
	virtual void	UnCheckMouseOut();
	virtual void	UnCheckLButtonDown();
	virtual void	UnCheckLButtonClick();


protected:
	

	UI_BUTTON_STATE m_eButtonState;
	_bool		m_bClickToggle;
	BUTTON_LEVEL	 m_eButtonLevel = BUTTON_LEVEL::LEVEL1;
};

END
