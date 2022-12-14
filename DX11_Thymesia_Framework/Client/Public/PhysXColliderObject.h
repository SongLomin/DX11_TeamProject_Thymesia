#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CPhysXCollider;
class CVIBuffer_Cube;
class CShader;
class CRenderer;
END

BEGIN(Client)

class CPhysXColliderObject :
    public CGameObject
{
    GAMECLASS_H(CPhysXColliderObject);
    CLONE_H(CPhysXColliderObject, CGameObject);
    SHALLOW_COPY(CPhysXColliderObject);

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);
    virtual _bool IsPicking(const RAY& In_Ray, _float& Out_fRange) override;

private:
    weak_ptr<CPhysXCollider>     m_pPhysXColliderCom;
    weak_ptr<CVIBuffer_Cube>     m_pVIBufferCom;
    weak_ptr<CShader>            m_pShaderCom;
    weak_ptr<CRenderer>          m_pRendererCom;
    _bool                        m_bViewPhysXInfo = false;
    _float4                      m_vColor = { 1.f, 0.f, 0.f, 1.f };

public:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;
    virtual void OnEventMessage(_uint iArg) override;

private:
    void Free();
};

END