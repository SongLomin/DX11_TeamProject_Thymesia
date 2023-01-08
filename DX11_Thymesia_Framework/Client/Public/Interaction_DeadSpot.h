#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CTexture;
END

BEGIN(Client)

class CInteraction_DeadSpot final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_DeadSpot);
    CLONE_H(CInteraction_DeadSpot, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override; 

public:
    void Init_DeadSpot(_vector In_vPos, _uint In_iMemory);

public:
    virtual void Act_Interaction() override;
    virtual void OnEventMessage(_uint iArg) override;

private:
    void    SetUpColliderDesc();

private:
    weak_ptr<CCollider>     m_pColliderCom;
    weak_ptr<CTexture>      m_pTextureCom;

    _uint                   m_iMemory     = 0;
    LIGHTDESC               m_tLightDesc;

    _uint m_iEffectIndex = 0;

private:
    virtual void OnDestroy() override;
    void Free();
};

END