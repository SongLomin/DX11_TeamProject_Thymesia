#pragma once
#include "RequirementBase.h"

BEGIN(Client)

class CRequirement_Time :
    public CRequirementBase
{
    // CRequirementBase을(를) 통해 상속됨
public:
    virtual _bool Check_Requirement() override;
    virtual _bool Is_Valid() override;

    void Init_Req(const _float In_fReqTime);

    _float  Get_RatioTime();
    
private:
    _float  m_fReqTime = 0.f;
    _float  m_fReqOriginTime = 0.f;

public:
    virtual HRESULT     Initialize(void* pArg) override;
    virtual void Free() override;

};

END