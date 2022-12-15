#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CPhysXCollider;
END


BEGIN(Client)

class CStatic_Prop final :
    public CProp
{
    GAMECLASS_H(CStatic_Prop);
    CLONE_H(CStatic_Prop, CGameObject);

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
    virtual _bool IsPicking(const RAY& In_Ray, _float& Out_fRange) override;
    virtual void OnEventMessage(_uint iArg) override;

private:
    virtual HRESULT SetUp_ShaderResource() override;

    virtual void Load_FromJson(const json& In_Json) override;
    virtual void Write_Json(json& Out_Json) override;

private:
    weak_ptr<CPhysXCollider>    m_pPhyxXColliderCom;

    _uint                       m_iColliderType = 0;
    _float                      m_fModelMidOffset;

private:
    void Free();
};

END