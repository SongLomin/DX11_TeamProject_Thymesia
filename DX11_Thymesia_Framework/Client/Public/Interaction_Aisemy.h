#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CCollider;
class CPhysXCollider;

END

BEGIN(Client)

class CInteraction_Aisemy final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_Aisemy);
    CLONE_H(CInteraction_Aisemy, CGameObject);

private:
    enum ANIM_INDEX 
    {
        SIT_NO1
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
    void    SetUpColliderDesc(weak_ptr<CCollider> In_pColldierCom, _float* _pColliderDesc, COLLISION_LAYER _eCollLayer);
    void    SetAnimState(const ANIM_INDEX _eAnimIndex);

private:
    weak_ptr<CCollider>         m_pColliderCom;
    weak_ptr<CPhysXCollider>    m_pPhysXColliderCom;
                                                      
private:
    void Free();
};

END

