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
    struct STATUS_DESC
    {
        _float fMaxHP = 0.f;
        _float fCurrentHP = 0.f;
        _float fRedRatio = 0.f;
        _float fDashRatio = 0.f;
        _float fMaxSwapTime = 15.f;
        _float fSwapTime = 0.f;
        _float fSPRatio = 0.f;
        _float fUltimateRatio = 0.f;
        tstring szName;
    };

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;

    

public:
    STATUS_DESC Get_Desc() const { return m_tStatusDesc; }
    _float  Get_HP() const { return m_tStatusDesc.fCurrentHP; }
    _float  Get_HPRatio() const { return m_tStatusDesc.fCurrentHP / m_tStatusDesc.fMaxHP; }
    void    Set_FullHP() { m_tStatusDesc.fCurrentHP = m_tStatusDesc.fMaxHP; }
    

    void Set_RedRatio(const _float& In_fRedRatio) { m_tStatusDesc.fRedRatio = In_fRedRatio; }
    _float  Get_RedRatio() const { return m_tStatusDesc.fRedRatio; }

    void Set_DashRatio(const _float& In_fDashRatio) { m_tStatusDesc.fDashRatio = In_fDashRatio; }

    _bool   Is_Dead() { return m_tStatusDesc.fCurrentHP <= DBL_EPSILON; };
    _bool   Requirment_Dash() { return m_tStatusDesc.fDashRatio > 0.f; }


public:
    void Init_Status(const STATUS_DESC& In_tStatusDesc);

    void Add_Damage(const _float& In_fDamage);
    void Heal_HP(const _float& In_fHP);
    void Add_DashRatio(const _float& In_fDashRatio);
    void On_Swap();

    void Add_SwapTime(const _float& In_fTimeDelta);

private:
    STATUS_DESC m_tStatusDesc;

private:
    virtual void Free() override;

};

END