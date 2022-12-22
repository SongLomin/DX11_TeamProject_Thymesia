#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"

BEGIN(Client)

class CInteraction_Dynamic_Prop final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_Dynamic_Prop);
    CLONE_H(CInteraction_Dynamic_Prop, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override; 

private:
    void Free();
};

END