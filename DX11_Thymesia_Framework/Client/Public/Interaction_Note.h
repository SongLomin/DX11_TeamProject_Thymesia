#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"
#include "ClientLevel.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CInteraction_Note final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_Note);
    CLONE_H(CInteraction_Note, CGameObject);

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
    void    SetUpColliderDesc(_float* _pColliderDesc);

private:
    weak_ptr<CCollider>     m_pColliderCom;

private:
    void Free();
};

END