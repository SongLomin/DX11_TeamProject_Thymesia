#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CCollider;
class CPhysXCollider;

END

BEGIN(Client)

class CInteraction_Fence final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_Fence);
    CLONE_H(CInteraction_Fence, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

private:
    void Act_OpenDoor(_float fTimeDelta, _bool& Out_IsEnd);
    void Act_CloseDoor(_float fTimeDelta, _bool& Out_IsEnd);

public:
    virtual void Act_Interaction() override;

public:
    virtual void OnEventMessage(_uint iArg) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

private:
    void    SetUpColliderDesc(weak_ptr<CCollider> In_pColldierCom, _float* _pColliderDesc, COLLISION_LAYER _eCollLayer);

private:
    weak_ptr<CCollider>         m_pColliderCom;
    weak_ptr<CCollider>         m_pDirColliderCom;
    weak_ptr<CPhysXCollider>    m_pPhysXColliderCom;

    _bool                       m_ActionFlag        = true;
    _float                      m_fMoveDistance     = XMConvertToRadians(90.f);
    _float                      m_fMovetSpeed       = 1.f;

    _float                      m_fAddDistance      = 0.f;
    _float4                     m_vFirstPosition    = { 0.f, 0.f, 0.f, 1.f };

    _int                        m_iSectionIndex     = -1;

private:
    virtual void OnDestroy() override;
    void Free();
};

END

