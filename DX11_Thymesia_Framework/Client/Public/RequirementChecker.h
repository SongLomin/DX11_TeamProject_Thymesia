#pragma once
#include "ClientComponent.h"
#include "Requirement_Once.h"
#include "Requirement_State.h"

BEGIN(Client)

class CRequirementChecker :
    public CClientComponent
{
    GAMECLASS_H(CRequirementChecker)
    SHALLOW_COPY(CRequirementChecker)
    CLONE_H(CRequirementChecker, CComponent)


public:
    _size_t size() const { return m_pRequirements.size(); }

protected:
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;

    

public:
    void    Add_Requirement(shared_ptr<CRequirementBase> pRequirment);
    _bool   Check_Requirments();

private:
    void    Update_Vaild();

private:
    list<shared_ptr<CRequirementBase>> m_pRequirements;

private:
    void Free();

};

END