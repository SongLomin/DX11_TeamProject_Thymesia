#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Client)

class CStatic_Prop final :
    public CProp
{
    GAMECLASS_H(CStatic_Prop);
    CLONE_H(CStatic_Prop, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    void Free();
};

END