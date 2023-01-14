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
        HEIGHT_SET,
        FILLTER,
        NON,
        EDIT_END
    };

    enum class BRUSH_MODE
    {
        BRUSH_NO1,
        BRUSH_NO2,
        BRUSH_NO3,
        CLEAR,

        BRUSH_END
    };

    typedef struct tag_TextureInfo
    {
        weak_ptr<CTexture>       pDiffTex;
        weak_ptr<CTexture>       pNormTex;
        weak_ptr<CTexture>       pORMTex;
        _float                   fDensity  = 30.f;
    } TEXTURES_INFO;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

private:
    HRESULT SetUp_ShaderResource();
    void    Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName = "");
    _bool   Check_File(const string& In_Path);

    void    SetUp_EditMode();
    void    SetUp_Info();
    void    SetUp_ShaderComponent();
    void    SetUp_PinckingInfo();
    void    SetUp_Textures();
    void    SetUp_File();

    void    CreateBuffer();
    void    CreateFilterTexture();

    void    PickingGround();
    void    PickingFillterTextureDraw();

    void    Bake_Mesh();
    void    Load_Mesh();
    void    Bake_FilterTexture();
    void    Load_FilterTexture();

public:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

public:
    static void    Load_AllMeshInfo();
    _bool          Load_AddMeshInfo(string _szFileName);
    _bool          Load_AddTextureInfo(string _szFileName);
    void           Load_FilterTexture_FromJson(string _szFileName);

public:
    virtual void   OnEventMessage(_uint iArg) override; 

private:
    typedef map<string, TEXTURES_INFO>  TEXTURES;
    typedef vector<string>              TEXTURE_NAMES;

    weak_ptr<CShader>                   m_pShaderCom;
    weak_ptr<CRenderer>                 m_pRendererCom;
    weak_ptr<CVIBuffer_Ground>          m_pVIBufferCom;
    TEXTURES                            m_pTextureCom;

    EDIT_MODE                           m_eEditMode           = EDIT_MODE::NON;
    _bool                               m_bEdit               = false;
    _bool                               m_bCreate             = false;
    _float4                             m_vBufferInfo         = _float4(128.f, 128.f, 0.5f, 0.f);
    _int                                m_iShaderPass         = 0;

    _float                              m_fBufferDrawRadious  = 1.f;
    _float                              m_fBufferPower        = 1.f;
    string                              m_szMeshName          = "";

    BRUSH_MODE                          m_eBrushMode          = BRUSH_MODE::BRUSH_NO1;
    string                              m_szSaveTextureTag    = "";
    ComPtr<ID3D11Texture2D>             m_pTexture2D;
    ComPtr<ID3D11ShaderResourceView>    m_pFilterTexture;
    vector<_uint>                       m_vColors;

    TEXTURE_NAMES                       m_TextureNames;
    TEXTURE_NAMES                       m_FilterNames;
    TEXTURE_NAMES                       m_MeshNames;

public:
    void Free();
};

END