#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Client)

class CPreView_InteriorProp final :
    public CProp
{
    GAMECLASS_H(CPreView_InteriorProp);
    CLONE_H(CPreView_InteriorProp, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    _bool Get_CheckBuild() { return m_bCheckBuild; }
    weak_ptr<CGameObject> Get_CollisionObject() { return m_pCollisionObject; }
 
    void Set_Offset(_vector _vOffset);
    void Set_Color(_fvector _vColor) { XMStoreFloat4(&m_vColor, _vColor); }
    void Set_Model(string _szModelKey);

public:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider);
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider);
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider);

private:
    virtual HRESULT SetUp_ShaderResource() override;

private:
    weak_ptr<CCollider>         m_pColliderCom;

    weak_ptr<CGameObject>       m_pCollisionObject;

    _float3  m_vOffset     = { 0.f, 0.f, 0.f };
    _float4  m_vColor      = { 1.f, 0.f, 0.f, 0.5f };
    _float   m_fPhyxOffset = 0.f;
    _bool    m_bCheckBuild = true;


public:
    void Free();
};

END