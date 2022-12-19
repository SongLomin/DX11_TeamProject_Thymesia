#pragma once
#include "ClientComponent.h"
#include "Requirement_Once.h"
#include "Requirement_State.h"

BEGIN(Client)

class CRequirementChecker :
    public CBase
{
    GAMECLASS_H(CRequirementChecker)
    SHALLOW_COPY(CRequirementChecker)


public:
    _size_t size() const { return m_pRequirements.size(); }

protected:
    HRESULT Initialize(void* pArg);

public:
    void    Add_Requirement(shared_ptr<CRequirementBase> pRequirment);
    _bool   Check_Requirments();
    void    Clear_Requirements();
private:
    void    Update_Vaild();
    
private:
    list<shared_ptr<CRequirementBase>> m_pRequirements;

public:
    static  shared_ptr<CRequirementChecker> Create(void* pArg = nullptr);

private:
    void Free();

};

END