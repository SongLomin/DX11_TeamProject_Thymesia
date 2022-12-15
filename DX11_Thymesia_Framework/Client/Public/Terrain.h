#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Terrain;
class CPhysXCollider;
END

BEGIN(Client)

class CTerrain :
    public CGameObject
{
    GAMECLASS_H(CTerrain)
    CLONE_H(CTerrain, CGameObject)

public:

    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

private:
    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;
    weak_ptr<CTexture> m_pTextureCom;
    weak_ptr<CVIBuffer_Terrain> m_pVIBufferCom;
    weak_ptr<CPhysXCollider> m_pPhyxXColliderCom;

private:
    HRESULT SetUp_ShaderResource();

protected:
    void Free();

};

END