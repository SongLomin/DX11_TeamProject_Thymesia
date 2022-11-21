#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Terrain;
class CModel;
END

BEGIN(Client)

class CProp :
    public CGameObject
{
    GAMECLASS_H(CProp);
    CLONE_H(CProp, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Custom_Thread1(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    void Set_ShaderPass(_bool _iIndex) { m_iPassIndex = _iIndex; }

protected:
    virtual void SetUp_ShaderResource();

protected:
    weak_ptr<CModel>    m_pModelCom;
    weak_ptr<CShader>   m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;

protected:
    RENDERGROUP m_eRenderGroup = RENDERGROUP::RENDER_NONALPHABLEND;
    _uint       m_iPassIndex = 0;
    _float      m_fCullingOffsetRange = 0.f;
    _bool       m_bRendering = true;

protected:
    void Free();

};

END