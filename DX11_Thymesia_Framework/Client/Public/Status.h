#pragma once
#include "ClientComponent.h"

BEGIN(Client)

class CStatus :
    public CClientComponent
{
    GAMECLASS_H(CStatus)
    SHALLOW_COPY(CStatus)
    CLONE_H(CStatus, CComponent)


protected:
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;

public: 
    virtual _bool   Is_Dead();
    virtual void    Init_Status(const void *pArg);
    virtual void    Add_Damage(const _float In_fDamage, ATTACK_OPTION eAttackOption);
    virtual void    Get_Desc(void* Out_pDesc);
    virtual void    Set_Desc(void* In_Desc);
    virtual void    Full_Recovery();
protected:
    virtual void    Decrease_HP(_float& InOut_fCurrentHP, const _float In_fDamage);
    virtual void    Increase_HP(_float& InOut_fCurrentHP,const _float In_fMaxHP, const _float In_fAmount);


private:
    void Free();


    

};

END