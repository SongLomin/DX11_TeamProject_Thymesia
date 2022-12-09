#pragma once
#include "Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CRequirementBase abstract :
    public CBase
{
    GAMECLASS_H(CRequirementBase)

public:
    CRequirementBase(void* pArg);

public:
    virtual _bool       Check_Requirement() PURE;
    virtual _bool       Is_Valid() PURE;

protected:
    virtual HRESULT     Initialize(void* pArg = nullptr) { return S_OK; }

protected:
    virtual void Free() {};

};

END