#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CCollider;
class CPhysXCollider;

END

BEGIN(Client)

class CInteraction_Door final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_Door);
    CLONE_H(CInteraction_Door, CGameObject);

private:
    enum ACTION_FLAG
    {
        ROTATION  = (1 << 0),   // 1

        KEY       = (1 << 1),   // 2
        AUTO      = (1 << 2),   // 4
        OPEN_DIR  = (1 << 3),   // 8
        LOCK      = (1 << 4)    // 16
    };

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Thread_PreBeforeRender(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    
private:
    void Act_OpenDoor(_float fTimeDelta, _bool& Out_IsEnd);
    void Act_CloseDoor(_float fTimeDelta, _bool& Out_IsEnd);

    void Requirement_Key(_bool& Out_bRequirement);
    void Requirement_Dir(_bool& Out_bRequirement);

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
         
    _flag                       m_ActionFlag        = ACTION_FLAG::ROTATION;
    _float                      m_fRotationtRadian  = XMConvertToRadians(90.f);
    _float                      m_fRotationtSpeed   = 1.f;
    _float                      m_fAddRadian        = 0.f;
    _float                      m_fFirstRadian      = 0.f;
    ITEM_NAME                   m_iKeyID            = ITEM_NAME::ITEM_NAME_END;
    _int                        m_iSectionIndex     = -1;

    _float4x4                   m_FirstMatrix;

private:
    virtual void OnDestroy() override;
    void Free();
};

END

