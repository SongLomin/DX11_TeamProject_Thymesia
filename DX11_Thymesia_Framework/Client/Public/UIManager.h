#pragma once
#include "Client_Defines.h"

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

    void        OnEnterStage();

    void        Initalize();

public:
    void Tick(_float fTimeDelta);
    void LateTick(_float fTimeDelta);

public:// For UI;
    POINT   Get_MousePoint();

public:
    void    SetCursor(weak_ptr<CUI_Cursor> pCursor) { m_pCursor = pCursor; }
    void    EnableCursor();
    void    DisableCursor();

    void      Add_SoundType(UI_SOUND_TYPE eType, _float fAmount);
    _float    Get_SoundType(UI_SOUND_TYPE eType);



    void        Set_CloseCurtain(_float fTime);
    FDelegate<> Callback_OpenCurtain;

private:
    void        OpenCurtain();


public:
    BUTTON_LEVEL        Get_ButtonLevel() { return m_eButtonLevel; }
    void                Set_ButtonLevel(BUTTON_LEVEL eButtonLevel) { m_eButtonLevel = eButtonLevel; }

    void                Set_Complete_SetUpUI() { m_bCompleteSetUpUI = true; }
    _bool               Get_Completed_SetUpUI() { return m_bCompleteSetUpUI; }

public:
    void               OnEnterEvolveMenu();
    void               OnExitEvolveMenu();

public:
    void   CreateItemPopupQueue();
    void   Add_ItemPopup(ITEM_NAME eItemName);

private:
    weak_ptr< CUI_Cursor> m_pCursor;
    shared_ptr<CItemPopup_Queue> m_pItemPopupQueue;
    BUTTON_LEVEL  m_eButtonLevel = BUTTON_LEVEL::LEVEL1;


public:
    void        Open_EvolveMenu();

private:
    _float      m_fCurtainTime;


private:
    POINT       m_MousePt;

private:
	_bool		m_bOpenedMenu = false;
	_bool		m_bIsAnimation = false;

    _bool       m_bCompleteSetUpUI = false;


    typedef map<UI_SOUND_TYPE, _float>  UISOUNDTYPE;
    map<UI_SOUND_TYPE, _float> m_UISoundTypeMap;


private:
	void		Free();

};
END
