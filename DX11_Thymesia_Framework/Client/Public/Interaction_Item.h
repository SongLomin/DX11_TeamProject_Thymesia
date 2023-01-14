#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"
#include "ClientLevel.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CInteraction_Item final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_Item);
    CLONE_H(CInteraction_Item, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override; 

public:
    void Add_Item(ITEM_NAME In_eItemType) { m_Items.push_back(In_eItemType); }

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

    list<ITEM_NAME>         m_Items;
    _int                    m_iEffectIndex = -1;

private:
    virtual void OnDestroy() override;
    void Free();
};

END