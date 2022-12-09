#pragma once
#include "RequirementBase.h"

BEGIN(Client)


class CRequirement_Once :
    public CRequirementBase
{
    // CRequirementBase을(를) 통해 상속됨
public:
    virtual _bool Check_Requirement() override;
    virtual _bool Is_Valid() override;

private:
    virtual HRESULT Initialize(void* pArg = nullptr) override;
    virtual void Free() override;
};

END