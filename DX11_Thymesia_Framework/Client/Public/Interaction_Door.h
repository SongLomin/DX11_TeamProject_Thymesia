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
        ACTIVATE = ( 1 << 0 ), // 1
        ROTATION = ( 1 << 1 ), // 2
    };

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override; 

public:
    virtual void Act_Interaction() override;

public:
    virtual void OnEventMessage(_uint iArg) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

private:
    void    SetUpColliderDesc(_float* _pColliderDesc);

private:
    weak_ptr<CCollider>         m_pColliderCom;
    weak_ptr<CPhysXCollider>    m_pPhysXColliderCom;
         
    _flag                       m_ActionFlag        = ACTION_FLAG::ROTATION;
    _float                      m_fRotationtRadian  = XMConvertToRadians(90.f);
    _float                      m_fRotationtSpeed   = 1.f;
    _float                      m_fAddRadian        = 0.f;
    _float                      m_fFirstRadian      = 0.f;
};

END

