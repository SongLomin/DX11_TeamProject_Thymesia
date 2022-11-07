#pragma once
#include "Component.h"
#include "Client_Defines.h"

BEGIN(Client)

class CClientComponent :
    public CComponent
{
    GAMECLASS_H(CClientComponent);

public:
    virtual void OnEditerView() {}

};

END