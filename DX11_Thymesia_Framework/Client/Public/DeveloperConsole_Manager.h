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
    void    Background_Tick(_float fTimeDelta);
    HRESULT Render(ID3D11DeviceContext* pDeviceContext);



private:
    void    Init_Windows();
    void    Release_Windows();

public:
    vector<shared_ptr<CImGui_Window>> m_arrWindows;

public:
    void OnEnableConsole(const _bool In_bEnable);

private:
    void Free();

};

END