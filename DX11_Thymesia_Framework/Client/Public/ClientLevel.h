#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CFadeMask;


class CClientLevel abstract :
    public CLevel
{
    GAMECLASS_H(CClientLevel)

protected:
    virtual HRESULT Initialize() override;
    void Load_FromJson(const string& In_szJsonPath, const LEVEL& In_eLevel);

public:
    void Change_NextLevel(void* pArg);

protected:
    string  m_szDefaultJsonPath = "../Bin/LevelData/";
    LEVEL   m_eMyLevel = LEVEL::LEVEL_END;
    LEVEL   m_eNextLevel = LEVEL::LEVEL_END;

protected:
    weak_ptr<CFadeMask> m_pFadeMask;
    _bool m_bFadeOut = false;

    _int m_iMonsterCount = 0;

    _bool m_bChangeNextLevel = false;

public:
    void Call_FadeOutToLevelChange();

protected:
    void Free();

};

END