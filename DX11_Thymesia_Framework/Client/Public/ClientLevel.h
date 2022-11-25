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
protected:
    void	Loading_AllEffectGroup(const char* In_FolderPath, const _uint& In_LevelIndex);


    virtual void		SetUp_UI();

protected:
    string  m_szDefaultJsonPath = "../Bin/LevelData/";
    LEVEL   m_eMyLevel = LEVEL::LEVEL_END;
    LEVEL   m_eNextLevel = LEVEL::LEVEL_END;

protected:
    weak_ptr<CFadeMask> m_pFadeMask;
    _bool m_bFadeOut = false;

    _int m_iMonsterCount = 0;

    _bool m_bChangeNextLevel = false;


protected:
    weak_ptr<CUI_PauseMenu> m_pPauseMenu;
    weak_ptr<CUI_EvolveMenu> m_pEvolveMenu;


public:
    void Call_FadeOutToLevelChange();




public:
    void		Call_Enable_PauseMenu();
    void		Call_Enable_EvolveMenu();



protected:
    void Free();

};

END