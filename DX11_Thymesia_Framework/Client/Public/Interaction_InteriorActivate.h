#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"
#include <Client_GameObjects.h>

BEGIN(Engine)

class CCollider;
class CPhysXCollider;
class CActorDecor;

END

BEGIN(Client)

class CInteraction_InteriorActivate final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_InteriorActivate);
    CLONE_H(CInteraction_InteriorActivate, CGameObject);

private:
    enum ANIM_EVENT
    {
        STAY            = 1,
        EQUIP_END       = 4,
        EQUIP_BEGINE    = 5,
        EQUIP_LOOP      = 6,
    };

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Thread_PreTick(_float fTimeDelta) override;
    virtual void Thread_PreLateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

private:
    HRESULT SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext);

public:
    virtual void Act_Interaction() override;

public:
    virtual void OnEventMessage(_uint iArg) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

private:
    void    SetUpColliderDesc();
    void    SetAnimState(const ANIM_EVENT _eAnimIndex);

    void    Call_CheckAnimEnd();
    void    Call_PlayAnimState();
    void    Call_ActivateInterior();

private:
    weak_ptr<CCollider>         m_pColliderCom;
    weak_ptr<CModel>            m_pAnimModelCom;
    weak_ptr<CTexture>          m_pTextureCom; 
    weak_ptr<CPhysXCollider>    m_pPhysXColliderCom;

    weak_ptr<CActorDecor>       m_pDeco;

    ANIM_EVENT                  m_eAnimEvent    = ANIM_EVENT::STAY;
    _int                        m_iTexPass      = 84;
    _float2                     m_vAddSpeed     = { -0.2f, 0.2f };
    _float                      m_fAisemyOffset = 0.2f;
    _float                      m_fAnimSpeed    = 1.f;
    _int                        m_iAnimIndex    = 0;
                
    _float2                     m_vAddUVPos     = { 0.f, 0.f };
    _bool                       m_bAnimEnd      = false;
    _bool                       m_bClear        = false;
    FDelegate<>                 CallBack_AnimPlay;
    FDelegate<>                 CallBack_Activate;

private:
    virtual void OnDestroy() override;
    void Free();
};

END

