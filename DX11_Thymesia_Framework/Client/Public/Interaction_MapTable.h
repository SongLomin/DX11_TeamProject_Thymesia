#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"
#include "ClientLevel.h"


BEGIN(Engine)
class CCollider;
class CPhysXCollider;
END

BEGIN(Client)

class CInteraction_MapTable final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_MapTable);
    CLONE_H(CInteraction_MapTable, CGameObject);

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
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

private:
    void    SetUpColliderDesc();

private:
    weak_ptr<CCollider>         m_pColliderCom;
    weak_ptr<CCollider>         m_pInteractionColliderCom;
    weak_ptr<CPhysXCollider>    m_pPhysXColliderCom;

    _float                      m_fPhyxOffset = 0.f;


private:
    virtual void OnDestroy() override;
    void Free();
};

END