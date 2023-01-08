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

class CInteriorProp :
    public CGameObject
{
    GAMECLASS_H(CInteriorProp);
    CLONE_H(CInteriorProp, CGameObject);
     
private:
    enum PROP_ID 
    {
        PID_ALCHEMY,
        SKILL_TABLE,
        ABILITY_TABLE
    };

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

private:
    virtual HRESULT SetUp_ShaderResource();

    void Edit_Props();

private:
    weak_ptr<CShader>               m_pShaderCom;
    weak_ptr<CRenderer>             m_pRendererCom;
    weak_ptr<CVIBuffer_Ground>      m_pVIBufferCom;

private:
    RENDERGROUP m_eRenderGroup          = RENDERGROUP::RENDER_NONALPHABLEND;

protected:
    void Free();
};

END