#pragma once
#include "UI_Container.h"


BEGIN(Engine)

ENGINE_DECLATION_UI;

END

BEGIN(Client)

CLIENT_DECLATION_UI

class CFadeMask;
class CUI_EvolveLeftBG;
class CUI_EvolveMenu_SelectDecoration;

class CUI_EvolveMenu final : public CUI_Container
{

public:

    enum class EVOLVEMENU_TYPE {
        EVOLVE_LEVELUP = 0,
        EVOLVE_UNLOCKTALENT, 
        EVOLVE_PLAGUEWEAPON,
        EVOLVE_CEASE_RECALL,
        EVOLVE_RESUME_GAME,
        EVOLVE_END
    };
public:
    GAMECLASS_H(CUI_EvolveMenu)
    CLONE_H(CUI_EvolveMenu, CGameObject)

public:
    virtual HRESULT Initialize(void* pArg) override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;

    virtual void OnEnable(void* _Arg = nullptr);
    virtual void OnDisable() override;


    /*
    
    
    */

private: //Left UI

    weak_ptr<CUI_EvolveLeftBG> m_pLeftBG;
    weak_ptr<CCustomUI> m_pSelectHighlight;

    weak_ptr<CCustomUI> m_pMenuTitleBG;
    weak_ptr<CCustomUI> m_pMenuTitleText;

    weak_ptr<CCustomUI>  m_pMenuText[(_uint)EVOLVEMENU_TYPE::EVOLVE_END];

    _bool               m_bOpenedPage[(_uint)EVOLVEMENU_TYPE::EVOLVE_END];
    _uint               m_iSelectedIndex;
    _char              m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_END][MAX_PATH];

    weak_ptr< CUI_EvolveMenu_SelectDecoration> m_pLeftSelectDecoration;


private://Right UI
    weak_ptr<CCustomUI> m_pRightBG;
    weak_ptr<CCustomUI> m_pRightTitle;
    weak_ptr<CCustomUI> m_pRightTitleDecoration;
    weak_ptr<CCustomUI> m_pRightMapImage;
    
    weak_ptr<CCustomUI> m_pQuestTitle;
    weak_ptr<CCustomUI> m_pQuestTitleDecoration;


    weak_ptr<CCustomUI> m_pQuestInformation;

    weak_ptr<CCustomUI> m_pQuestInformationsDecoration;


private:
    weak_ptr<CFadeMask> m_pFadeMask;

private:
   LEVEL                m_eLastOpenedLevel;
   _bool               m_bEnabledThisFrame;

private:
    void               ChangeButtonIndex();
    void               SelectButton();
    void               SetUpFromCurrentLevel();
    

private:
    virtual void Free() override;



    //Callback Fucntios
public:
    virtual void    Call_FadeEndEnableEvolveMenu();
    virtual void    Call_ChangeUI_EvolveMenu_Level();
    virtual void    Call_ChangeUI_EvolveMenu_Talent();
    virtual void    Call_ChangeUI_EvolveMenu_PlagueWeapon();
    virtual void    Call_ChangeUI_EvolveMenu_Option();






};

END