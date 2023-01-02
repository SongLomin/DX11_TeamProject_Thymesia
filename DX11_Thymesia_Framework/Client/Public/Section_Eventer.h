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

private:
    enum EVENT_FLAG
    {
        EVENT_ENTER         = (1 << 0),
        EVENT_STAY          = (1 << 1),
        EVENT_EXIT          = (1 << 2)
    };

    enum ACT_FLAG
    {
        ACT_SECTION         = (1 << 0),
        ACT_MONSTER_TRIGGER = (1 << 1),
        ACT_LIGHT           = (1 << 2),
        ACT_FOG             = (1 << 3)
    };

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    _int  Get_SectionIndex() { return m_iSectionIndex; }

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
#ifdef _DEBUG
    weak_ptr<CRenderer>   m_pRendererCom;
#endif

    weak_ptr<CCollider>   m_pColliderCom;  

    _flag                 m_EventFlag     = 0;
    _flag                 m_ActFlag       = 0;
    _int                  m_iSectionIndex = 0;
    _float4               m_vColor        = { 1.f, 0.f, 0.f, 1.f };

private:
    virtual void OnDestroy() override;
    void Free();
};

END

