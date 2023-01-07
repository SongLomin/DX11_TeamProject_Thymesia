#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"
#include "ClientLevel.h"


BEGIN(Engine)
class CCollider;
class CModel;
class CMobWeapon;
END

BEGIN(Client)

class CInteraction_CheckPoint final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_CheckPoint);
    CLONE_H(CInteraction_CheckPoint, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Thread_PreTick(_float fTimeDelta) override;
    virtual void Thread_PreLateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    virtual void Act_Interaction() override;

public:
    virtual void OnEventMessage(_uint iArg) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

private:
    void    SetUpColliderDesc();
    HRESULT Draw_Chair(ID3D11DeviceContext* pDeviceContext);
    HRESULT Draw_Aisemy(ID3D11DeviceContext* pDeviceContext);

private:
    weak_ptr<CCollider>     m_pColliderCom;
    weak_ptr<CModel>        m_pAnimModelCom;
    weak_ptr<CShader>       m_pAnimShader;
    weak_ptr<CTexture>      m_pTextureCom;
    weak_ptr<CMobWeapon>    m_pWeapon;

    _int                    m_iTexPass      = 84;
    _float2                 m_vAddSpeed     = { -0.2f, 0.2f };
    _float                  m_fAisemyOffset = 0.3f;
    _float                  m_fAnimSpeed    = 2.f;
    _int                    m_iAnimIndex    = 0;

    _int                    m_iCheckIndex   = 0;
    _uint                   m_iEffectIndex  = 0;
    LIGHTDESC               m_tLightDesc;   
    _float2                 m_vAddUVPos     = { 0.f, 0.f };

private:
    void Free();
};

END