#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CFadeMask;
class CUI_PauseMenu;
class CUI_EvolveMenu;


class CClientLevel abstract :
    public CLevel
{
    GAMECLASS_H(CClientLevel)

protected:
    virtual HRESULT Initialize() override;
    void Load_FromJson(const string& In_szJsonPath, const LEVEL& In_eLevel);

public:
    void Change_NextLevel(void* pArg);

public:
    LEVEL   Get_MyLevel() { return m_eMyLevel; }
    LEVEL   Get_NextLevel() { return m_eNextLevel; }

    static void	Loading_AllEffectGroup(const char* In_FolderPath, const _uint& In_LevelIndex);


    virtual void    Tick(_float fTimeDelta) override;
    virtual void    SetUp_UI();

    void    Tick_Key_InputEvent();
    void    Call_StageLanding();



protected:
    string  m_szDefaultJsonPath = "../Bin/LevelData/";
    LEVEL   m_eMyLevel = LEVEL::LEVEL_END;
    LEVEL   m_eNextLevel = LEVEL::LEVEL_END;

protected:
    weak_ptr<CFadeMask> m_pFadeMask;
    _bool m_bFadeOut = false;

    _int m_iMonsterCount = 0;

    _bool m_bChangeNextLevel = false;

    _bool m_bLading = false;

protected:
    weak_ptr<CUI_PauseMenu> m_pPauseMenu;
    weak_ptr<CUI_EvolveMenu> m_pEvolveMenu;

protected:
    _bool m_bFadeTrigger = false;


public:
    void Call_FadeOutToLevelChange();
    void SaveLevel();


public:
    virtual void OnLevelEnter() override;
    virtual void ExitLevel(LEVEL eLevel);


public:
    void		Call_Enable_PauseMenu();
    void		Call_Enable_EvolveMenu();
    void	    Call_FadeOutToStartGame();



protected:
    void Free();

};

END