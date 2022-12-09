#pragma once
#include "RequirementBase.h"

BEGIN(Client)
class CCharacter;
class CStateBase;

class CRequirement_State :
    public CRequirementBase
{
public:
    void    Init_Req(weak_ptr<CCharacter> pCharacter, const _uint In_StateIndex);

    // CRequirementBase을(를) 통해 상속됨
    virtual _bool Check_Requirement() override;
    virtual _bool Is_Valid() override;

private:
    weak_ptr<CCharacter>    m_pCharacter;
    _uint                   m_iStateIndex;
};

END