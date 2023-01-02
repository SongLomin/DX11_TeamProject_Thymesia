#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)

class CFog final :
    public CGameObject
{
    GAMECLASS_H(CFog);
    CLONE_H(CFog, CGameObject);

private:
    struct FOG_DESC 
    {
        _float4     vColor   = { 0.f, 0.f, 0.f, 0.f };
        _float      fRange   = 0.f;
        _float      fTime    = 0.f;
    };

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    void Activate_Fog(_uint _iFogIndex);

public:
    virtual void OnEventMessage(_uint iArg) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

private:
    void Act_Fog(_float fTimeDelta);

private:
    vector<FOG_DESC>        m_FogList;
    _bool                   m_bAction   = false;
    _uint                   m_iPreIndex = 0;
    _uint                   m_iCurIndex = 0;
    _float                  m_fAccTime  = 0.f;

    FDelegate<_float>       CallBack_Act;

private:
    virtual void OnDestroy() override;
    void Free();
};

END

