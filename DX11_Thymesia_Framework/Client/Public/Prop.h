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

    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    virtual void SetUp_ShaderResource();

protected:
    weak_ptr<CModel>    m_pModelCom;
    weak_ptr<CShader>   m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;

protected:
    RENDERGROUP m_eRenderGroup = RENDERGROUP::RENDER_NONALPHABLEND;
    _uint       m_iPassIndex = 0;

protected:
    void Free();

};

END