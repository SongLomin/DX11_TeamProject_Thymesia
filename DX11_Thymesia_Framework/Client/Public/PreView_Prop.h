#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Client)

class CPreview_Prop final :
    public CProp
{
    GAMECLASS_H(CPreview_Prop);
    CLONE_H(CPreview_Prop, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

private:
    _uint m_iContainerIndex=0;


public:
    void Free();
};

END