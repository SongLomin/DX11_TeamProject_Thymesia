#pragma once
#include "Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CImGui_Window;

class CDeveloperConsole_Manager :
    public CBase
{
    DECLARE_SINGLETON(CDeveloperConsole_Manager)

public:
    void    Initialize();
    void	Tick(_float fTimeDelta);
    HRESULT Render();

private:
    void    Init_Windows();
    void    Release_Windows();

public:
    vector<shared_ptr<CImGui_Window>> m_arrWindows;

private:
    void Free();

};

END