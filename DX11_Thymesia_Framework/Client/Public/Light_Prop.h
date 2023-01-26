#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CPhysXCollider; 
class CCollider;

END

BEGIN(Client)

class CLight_Prop final :
    public CProp
{
    GAMECLASS_H(CLight_Prop);
    CLONE_H(CLight_Prop, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    const LIGHTDESC& Get_LightDesc() { return m_tLightDesc; }
    void  Set_LightDesc(const LIGHTDESC& In_Desc);

public:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

    virtual void OnEventMessage(_uint iArg) override;

private:
    void Act_LightTurnOnEvent(_float fTimeDelta, _bool& Out_End);
    void Act_LightTurnOffEvent(_float fTimeDelta, _bool& Out_End);
    void Call_Activate_Sound();


private:
	LIGHTDESC   m_tLightDesc;
    _int        m_iSectionIndex    = -1;
	_float      m_fMinLightRange   = 0.f;
	_float      m_fMaxLightRange   = 0.f;
    _float3     m_vOffset          = { 0.f, 0.f, 0.f };
    _float      m_fDelayTime       = 0.f;
    _float      m_fIntensityTime   = 1.f;
    _float      m_fAccTime         = 0.f;

    string      m_szEffectTag      = "";
    _int        m_iEffectIndex     = -1;

    _float      m_fTargetIntensity = 0.f;
    _float      m_fTargetRange     = 0.f;
    _float      m_fDisableTime     = 0.5f;

    FDelegate<_float, _bool&>   Callback_ActUpdate;
    FDelegate<>                 Callback_OnActivate;

#ifdef _DEBUG
    weak_ptr<CCollider>   m_pColliderCom;

private:
    void    SetUpColliderDesc();
#endif

private:
    virtual void OnDestroy() override;
    void Free();
};

END