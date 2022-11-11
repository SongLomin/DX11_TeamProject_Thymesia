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

    enum class BRUSH_MODE
    {
        BRUSH_NO1,
        BRUSH_NO2,
        BRUSH_NO3,

        BRUSH_END
    };

    typedef struct tag_TextureInfo
    {
        weak_ptr<CTexture>       pDiffTex;
        weak_ptr<CTexture>       pNormTex;
        _int                     iType;
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

    void    SetUp_EditMode();
    void    SetUp_Info();
    void    SetUp_TextureComponent();
    void    SetUp_ShaderComponent();
    void    SetUp_PinckingInfo();
    void    SetUp_Textures();

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
    typedef map<string, TEXTURES_INFO>  TEXTURES;
    typedef vector<string>              TEXTURE_NAMES;

    weak_ptr<CShader>                   m_pShaderCom;
    weak_ptr<CRenderer>                 m_pRendererCom;
    weak_ptr<CVIBuffer_Ground>          m_pVIBufferCom;
    TEXTURES                            m_pTextureCom;

    //weak_ptr<CVIBuffer_Ground>  m_pVIBufferFilterCom;

    EDIT_MODE                           m_eEditMode = EDIT_MODE::NON;
    _bool                               m_bEdit = false;
    _bool                               m_bCreate = false;
    _float4                             m_vBufferInfo = _float4(128.f, 128.f, 0.5f, 0.f);
    _float                              m_fDensity = 30.f;
    _int                                m_iShaderPass = 0;

    _float                              m_fBufferDrawRadious = 1.f;
    _float                              m_fBufferPower = 1.f;
    string                              m_szMeshName = "";

    shared_ptr<MODEL_DATA>              m_pModelData;

    BRUSH_MODE                          m_eBrushMode = BRUSH_MODE::BRUSH_NO1;
    TEXTURE_NAMES                       m_TextureNames;
    ComPtr<ID3D11Texture2D>             m_pTexture2D;
    ComPtr<ID3D11ShaderResourceView>    m_pFilterTexture;
    vector<_uint>                       m_vColors;
    string                              m_szFindTextureTag = "";

public:
    void Free();
};

END