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

class CEditGround final :
    public CGameObject
{
    GAMECLASS_H(CEditGround)
    CLONE_H(CEditGround, CGameObject)
    SHALLOW_COPY(CEditGround)

private:
    enum class EDIT_MODE
    {
        HEIGHT_FLAT,
        HEIGHT_LERP,
        FILLTER,
        NON,
        EDIT_END
    };

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    HRESULT SetUp_ShaderResource();

    void    SetUp_EditMode();
    void    SetUp_Info();
    void    SetUp_TextureComponent();
    void    SetUp_ShaderComponent();
    void    SetUp_PinckingInfo();
    void    SetUp_FillterInfo();

    void    CreateBuffer();
    void    PickingGround();
    void    Bake_Mesh();
    void    Load_Mesh();

public:
    virtual void OnEventMessage(_uint iArg) override;

private:
    weak_ptr<CShader>           m_pShaderCom;
    weak_ptr<CRenderer>         m_pRendererCom;
    weak_ptr<CTexture>          m_pDiff_TextureCom;
    weak_ptr<CTexture>          m_pNorm_TextureCom;
    //vector < weak_ptr<CTexture>>   m_pFilterTextureCom;
    weak_ptr<CVIBuffer_Ground>  m_pVIBufferCom;

    _bool                       m_bEdit                 = false;
    _bool                       m_bCreate               = false;
    EDIT_MODE                   m_eEditMode             = EDIT_MODE::NON;
    _float4                     m_vBufferInfo           = _float4(10.f, 10.f ,1.f, 0.f);
    _float                      m_fDensity              = 30.f;
    _int                        m_iShaderPass           = 0;

    _float                      m_fFilterDrawRadious    = 1.f;
    _float                      m_fBufferDrawRadious    = 1.f;
    _float                      m_fBufferPower          = 1.f;
    string                      m_szMeshName;

public:
    void Free();
};

END