#pragma once
#include "Component.h"
#include "Client_Defines.h"

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
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

protected:
    string      m_szFieldName;


};

END