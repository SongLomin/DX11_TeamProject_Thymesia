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

class CEditMapTool final :
    public CGameObject
{
    GAMECLASS_H(CEditMapTool)
    CLONE_H(CEditMapTool, CGameObject)
    SHALLOW_COPY(CEditMapTool)

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
    void    CreateFilterTexture();

    void    PickingGround();
    void    PickingFillterTextureDraw();

    void    Bake_Mesh();
    void    Load_Mesh();
    void    Bake_FilterTexture();

public:
    static void    Load_AllMeshInfo();

public:
    virtual void OnEventMessage(_uint iArg) override;

private:
    typedef map<string, weak_ptr<CTexture>> TEXTURES;

    weak_ptr<CShader>           m_pShaderCom;
    weak_ptr<CRenderer>         m_pRendererCom;
    TEXTURES                    m_pTextureCom;
    weak_ptr<CVIBuffer_Ground>  m_pVIBufferCom;

    EDIT_MODE                   m_eEditMode             = EDIT_MODE::NON;
    _bool                       m_bEdit                 = false;
    _bool                       m_bCreate               = false;
    _float4                     m_vBufferInfo           = _float4(128.f, 128.f, 0.5f, 0.f);
    _float                      m_fDensity              = 30.f;
    _int                        m_iShaderPass           = 0;

    _float                      m_fBufferDrawRadious    = 1.f;
    _float                      m_fBufferPower          = 1.f;
    string                      m_szMeshName            = "";

    shared_ptr<MODEL_DATA>      m_pModelData;

    ComPtr<ID3D11Texture2D>               m_pTexture2D;
    ComPtr<ID3D11ShaderResourceView>      m_pFilterTexture;

public:
    void Free();
};

END