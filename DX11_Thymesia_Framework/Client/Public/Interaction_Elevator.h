#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CInteraction_Elevator final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_Elevator);
    CLONE_H(CInteraction_Elevator, CGameObject);

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
    void    SetUpColliderDesc();

private:
    weak_ptr<CCollider>     m_pColliderCom;

    LEVEL                   m_eNextLevel    = LEVEL::LEVEL_END;
    _float                  m_fColliderPosY = 1.5f;

private:
    void Free();
};

END