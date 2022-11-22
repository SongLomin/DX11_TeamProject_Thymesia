#pragma once
#include "ClientComponent.h"

BEGIN(Client)

class CStatus :
    public CClientComponent
{
    GAMECLASS_H(CStatus)
    SHALLOW_COPY(CStatus)
    CLONE_H(CStatus, CComponent)



public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;


protected:
    void Decrease_HP(_float& fCurrentHP, const _float& In_fDamage);

public:
    virtual _bool   Is_Dead();
    virtual void    Init_Status(const void *pArg);
    virtual void    Add_Damage(const _float& In_fDamage, ATTACK_OPTION eAttackOption);
    virtual _float    Get_Atk() { return 0.f; }
    virtual void     Get_Desc(void* Out_pDesc);
    
protected:
    void Free();

};

END