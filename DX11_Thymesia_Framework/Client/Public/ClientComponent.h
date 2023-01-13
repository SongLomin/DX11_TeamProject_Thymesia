#pragma once
#include "Component.h"
#include "Client_Defines.h"
#include "Player.h"

BEGIN(Client)

class CClientComponent :
    public CComponent
{
    GAMECLASS_H(CClientComponent);

public:
    virtual void Start() {};
public:
    virtual void OnEditerView() {}

public:
    virtual void    Write_Json(json& Out_Json) override;
    virtual void    Load_FromJson(const json& In_Json) override;

    virtual void    Write_SaveData(json& Out_Json);
    virtual void    Load_SaveData(const json& In_Json);


public: 
    virtual void    OnEventMessage(_uint iArg) override;


protected:
    string      m_szFieldName;


};

END