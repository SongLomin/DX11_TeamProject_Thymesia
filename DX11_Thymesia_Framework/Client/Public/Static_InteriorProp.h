#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CPhysXCollider;
END

BEGIN(Client)

class CStatic_InteriorProp final :
    public CProp
{
    GAMECLASS_H(CStatic_InteriorProp);
    CLONE_H(CStatic_InteriorProp, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext) override;

public:
    virtual void PhysXCollisionStay(weak_ptr<CPhysXCollider> pOtherCollider);

private:
    virtual HRESULT SetUp_ShaderResource() override;

    virtual void Load_FromJson(const json& In_Json) override;
    virtual void Write_Json(json& Out_Json) override;

private:
    weak_ptr<CPhysXCollider>    m_pPhysXColliderCom;     
    weak_ptr<CCollider>         m_pColliderCom;

    _float                      m_fModelMidOffset   = 0.f;
    _float                      m_fPhyxOffset       = 0.f;

private:
    void Free();
};

END