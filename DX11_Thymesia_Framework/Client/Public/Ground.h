#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Ground;
class CPhysXCollider;
END

BEGIN(Client)

class CGround final :
    public CGameObject
{
    GAMECLASS_H(CGround)
    CLONE_H(CGround, CGameObject)

private:
    typedef struct tag_TextureInfo
    {
        weak_ptr<CTexture>       pDiffTex;
        weak_ptr<CTexture>       pNormTex;
        weak_ptr<CTexture>       pORMTex;
        _float                   fDensity = 0.f;
    } TEXTURES_INFO;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    virtual void Load_FromJson(const json& In_Json) override;

private:
    HRESULT SetUp_ShaderResource();

private:
    typedef map<string, TEXTURES_INFO>  TEXTURES;

    weak_ptr<CTexture>              m_pNormTex;

	weak_ptr<CShader>               m_pShaderCom;
	weak_ptr<CRenderer>             m_pRendererCom;
	weak_ptr<CVIBuffer_Ground>      m_pVIBufferCom;
    weak_ptr<CPhysXCollider>        m_pPhysXColliderCom;

    shared_ptr<MESH_DATA>           m_pGroundMeshData;

    weak_ptr<CTexture>              m_pFilterTextureCom;
    TEXTURES                        m_pTextureDescs;

    _uint                           m_iShaderPath = 0;
    string                          m_szModelName;

public:
    void Free();
};

END