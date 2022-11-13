#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Ground;
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
        _float                   fDensity = 0.f;
    } TEXTURES_INFO;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    HRESULT SetUp_ShaderResource();

private:
    typedef map<string, TEXTURES_INFO>  TEXTURES;

	weak_ptr<CShader>               m_pShaderCom;
	weak_ptr<CRenderer>             m_pRendererCom;
	weak_ptr<CVIBuffer_Ground>      m_pVIBufferCom;
    TEXTURES                        m_pTextureCom;

public:
    void Free();
};

END