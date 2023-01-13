#pragma once
#include "UI_Container.h"
#include "UI_Script.h"


BEGIN(Client)


class CUI_ScriptQueue : public CUI_Container
{

public:
	GAMECLASS_H(CUI_ScriptQueue)
	CLONE_H(CUI_ScriptQueue, CGameObject)

public:
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;


public:
    void                    Call_SetScript_Tutorial_Varg();
    void                    Call_SetScript_Tutorial_Varg_Appear();
    void                    Call_SetScript_Varg_Page2();
    void                    Call_SetScript_Varg_Dead();

    void                    Call_SetScript_Bat_Appear();

    void                    Call_SetScript_Urd_Appear();
    void                    Call_SetScript_Urd_Phase2();
    void                    Call_SetScript_Urd_Dead();
    



private:
    void        OnEnable(void* pArg) override;
    void        OnDisable() override;
private:
    weak_ptr<CUI_Script>    m_pScript;
    queue<CUI_Script::SCRIPTDESC>  m_ScriptDescQueue;

private:
    virtual void Free();

};

END