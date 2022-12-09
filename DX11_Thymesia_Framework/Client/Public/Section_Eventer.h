#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CCollider;
class CShader;
class CRenderer;

END

BEGIN(Client)

class CSection_Eventer final :
    public CGameObject
{
    GAMECLASS_H(CSection_Eventer);
    CLONE_H(CSection_Eventer, CGameObject);

public:
    enum EVENT_FLAG 
    {
        ACT_SECTION         = (1 << 0),
        ACT_MONSTER_TRIGGER = (1 << 1),

        EVENT_ENTER         = (1 << 10),
        EVENT_STAY          = (1 << 11),
        EVENT_EXIT          = (1 << 12)
    };

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    virtual void OnEventMessage(_uint iArg) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

public:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider);
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider);
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider);

private:
    void    SetUpColliderDesc(_float* _pColliderDesc);

private:
    weak_ptr<CCollider>   m_pColliderCom;  

    _flag                 m_Flag          = 0;
    _int                  m_iSectionIndex = 0;
    _float4               m_vColor        = { 1.f, 0.f, 0.f, 1.f };
};

END

