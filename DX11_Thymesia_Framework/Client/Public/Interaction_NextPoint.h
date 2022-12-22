#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"
#include "ClientLevel.h"


BEGIN(Engine)
class CCollider;
class CTexture;
END

BEGIN(Client)

class CInteraction_NextPoint final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_NextPoint);
    CLONE_H(CInteraction_NextPoint, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override; 

public:
    virtual void Act_Interaction() override;

public:
    virtual void OnEventMessage(_uint iArg) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

private:
    virtual HRESULT SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext);

private:
    void    SetUpColliderDesc();

private:
    weak_ptr<CCollider>     m_pColliderCom;
    weak_ptr<CTexture>      m_pTextureCom;
    
    _int                    m_iTexPass      = 882;
    _float2                 m_vAddSpeed     = { -0.5f, 0.5f };

    LEVEL                   m_eNextLevel    = LEVEL::LEVEL_END;
    _int                    m_iSectionIndex = 1000;
    _float2                 m_vAddUVPos     = { 0.f, 0.f };
    LIGHTDESC               m_tLightDesc;

private:
    virtual void OnDestroy() override;
    void Free();
};

END