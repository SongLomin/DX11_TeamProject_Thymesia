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
    virtual HRESULT Render() override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix);

public:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

private:
    typedef vector<INSTANCE_MESH_DESC>    PROP_INFO;

    weak_ptr<CVIBuffer_Model_Instance>  m_pInstanceModelCom;
    PROP_INFO                           m_pPropInfos;
    weak_ptr<CPhysXCollider>            m_pPhysXColliderCom;

    _int                                m_iColliderType = 0;
    _bool                               m_bEdit = false;

public:
    void Free();
};

END