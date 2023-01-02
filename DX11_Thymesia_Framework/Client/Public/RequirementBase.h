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

public:
    FDelegate<>         Callback_OnRequirementTrue;


protected:
    virtual HRESULT     Initialize(void* pArg = nullptr);



protected:
    virtual void Free() {};

public:
    template <class  T >
    static shared_ptr<T> Create(void* pArg = nullptr)
    {
        shared_ptr<T> pRequirement = make_shared<T>();

        pRequirement->Initialize(pArg);

        return pRequirement;
    }
};

END