#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CFader;
END

BEGIN(Client)

class CWorldUI :
    public CGameObject
{
    GAMECLASS_H(CWorldUI)
    CLONE_H(CWorldUI, CGameObject)

public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    void Set_TextureIndex(const _uint& Index) { m_iTextureIndex = Index; }
    void Set_Texture(const _char* sKey);

protected:
    virtual HRESULT SetUp_ShaderResource();

protected:
    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;
    weak_ptr<CTexture> m_pTextureCom;
    weak_ptr<CVIBuffer_Rect> m_pVIBufferCom;
    weak_ptr<CFader> m_pFaderCom;

protected:
    RENDERGROUP     m_eRenderGroup = RENDERGROUP::RENDER_UI;
    _uint           m_iTextureIndex = 0;
    _uint           m_iPassIndex = 0;

    _float4x4       m_OffsetMatrix;

public:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

public:
    virtual void OnEventMessage(_uint iArg) override;

protected:
    void Free();

};

END