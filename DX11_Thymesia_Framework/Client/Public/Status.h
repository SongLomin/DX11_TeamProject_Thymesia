#pragma once
#include "ClientComponent.h"

BEGIN(Client)

class CStatus :
    public CClientComponent
{
    GAMECLASS_H(CStatus)
    SHALLOW_COPY(CStatus)
    CLONE_H(CStatus, CComponent)



private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;

public:
    virtual _bool   Is_Dead();
    virtual void    Init_Status(const void *pArg);
    virtual void    Add_Damage(const _float& In_fDamage, ATTACK_OPTION eAttackOption);
    
protected:
    void Free();

};

END