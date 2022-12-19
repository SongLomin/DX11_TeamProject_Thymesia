#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)

class CEditEventContoller final :
    public CGameObject
{
    GAMECLASS_H(CEditEventContoller)
    CLONE_H(CEditEventContoller, CGameObject)
    SHALLOW_COPY(CEditEventContoller)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    virtual void  OnEventMessage(_uint iArg) override;

private:
    void    View_SettingLight();
    void    View_SettingFrog();

    void    View_SelectEventLightType();
    void    View_SelectEventSectionType();

private:
    _int iSelectEventType = 0;
    _int iSelectSection   = 0;


public:
    void Free();
};

END