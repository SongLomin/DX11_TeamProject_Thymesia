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
    virtual void PhysXCollisionEnter(weak_ptr<CPhysXCollider> pOtherCollider);
    virtual void PhysXCollisionStay(weak_ptr<CPhysXCollider> pOtherCollider);
    virtual void PhysXCollisionExit(weak_ptr<CPhysXCollider> pOtherCollider);

public:
    _bool Get_CheckBuild() { return m_bCheckBuild; }

    void Set_Offset(_vector _vOffset);
    void Set_Color(_fvector _vColor) { XMStoreFloat4(&m_vColor, _vColor); }
    void Set_Model(string _szModelKey);


private:
    virtual HRESULT SetUp_ShaderResource() override;

private:
    weak_ptr<CPhysXCollider>    m_pPhysXColliderCom;

    _float3  m_vOffset     = { 0.f, 0.f, 0.f };
    _float4  m_vColor      = { 1.f, 0.f, 0.f, 0.5f };
    _float   m_fPhyxOffset = 0.f;
    _bool    m_bCheckBuild = true;


public:
    void Free();
};

END