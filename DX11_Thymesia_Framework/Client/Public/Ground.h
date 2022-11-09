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
    weak_ptr<CShader>           m_pShaderCom;
    weak_ptr<CRenderer>         m_pRendererCom;
    weak_ptr<CTexture>          m_pDiff_TextureCom;
    weak_ptr<CTexture>          m_pNorm_TextureCom;
    weak_ptr<CVIBuffer_Ground>  m_pVIBufferCom;

public:
    void Free();
};

END