#pragma once
#include "Prop.h"
#include "Client_Defines.h"


BEGIN(Engine)
class CVIBuffer_Model_Instance;
class CPhysXCollider;
END

BEGIN(Client)

class CStatic_Instancing_Prop final :
    public CProp
{
    GAMECLASS_H(CStatic_Instancing_Prop);
    CLONE_H(CStatic_Instancing_Prop, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Custom_Thread1(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext);

public:
    virtual void Load_FromJson(const json& In_Json) override;

private:
    void Bake_PhysXColliderCom();
    void Bake_DynamicColliderComs();
    void Synchronize_DynamicColliderComs();

private:
    typedef vector<INSTANCE_MESH_DESC>    PROP_INFO;

    weak_ptr<CTexture>                  m_pMaskingTextureCom;
    weak_ptr<CVIBuffer_Model_Instance>  m_pInstanceModelCom;
    weak_ptr<CPhysXCollider>            m_pPhysXColliderCom;
    PROP_INFO                           m_pPropInfos;
    vector<weak_ptr<CPhysXCollider>>    m_pDynamicColliderComs;

    _int                                m_iColliderType  = 0;
    _bool                               m_bNonCulling    = false;
    _bool                               m_bDissolve      = false;
    _float                              m_fDissolveSpeed = 1.f;

public:
    void Free();
};

END