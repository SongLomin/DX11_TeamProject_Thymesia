#pragma once
#include "Base.h"

BEGIN(Client)


class CUI_Cursor;
class CItemPopup_Queue;


class CUIManager : public CBase
{
	DECLARE_SINGLETON(CUIManager)

public:
	void		Set_OpenedMenu(_bool bOpenedMenu);
	_bool		Is_OpenedMenu() { return m_bOpenedMenu; }


	void		Set_UIAnimation(_bool bUIAnimation) { m_bIsAnimation = bUIAnimation; }
	_bool		Is_Animation() { return m_bIsAnimation; }

    

public:// For UI;
    POINT   Get_MousePoint();

public:
    void    SetCursor(weak_ptr<CUI_Cursor> pCursor) { m_pCursor = pCursor; }
    void    EnableCursor();
    void    DisableCursor();

public:
    BUTTON_LEVEL        Get_ButtonLevel() { return m_eButtonLevel; }
    void                Set_ButtonLevel(BUTTON_LEVEL eButtonLevel) { m_eButtonLevel = eButtonLevel; }

public:
    void   CreateItemPopupQueue();
    void   Add_ItemPopup(ITEM_NAME eItemName);

private:
    weak_ptr< CUI_Cursor> m_pCursor;
    shared_ptr<CItemPopup_Queue> m_pItemPopupQueue;
    BUTTON_LEVEL  m_eButtonLevel = BUTTON_LEVEL::LEVEL1;


private:
	_bool		m_bOpenedMenu = false;
	_bool		m_bIsAnimation = false;
private:
	void		Free();

};


END
