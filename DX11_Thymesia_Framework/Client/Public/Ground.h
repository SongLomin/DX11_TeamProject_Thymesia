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

public:
    // 툴에서 save로 저장하는 시점에 한 번만 호출됨.
    virtual void Write_Json(json& Out_Json) override;
    
    // 툴이나 인게임에서 객체 생성되고 나서 한 번만 호출됨.
    virtual void Load_FromJson(const json& In_Json) override;


private:
    HRESULT SetUp_ShaderResource();

private:
    typedef map<string, TEXTURES_INFO>  TEXTURES;

	weak_ptr<CShader>               m_pShaderCom;
	weak_ptr<CRenderer>             m_pRendererCom;
	weak_ptr<CVIBuffer_Ground>      m_pVIBufferCom;
    TEXTURES                        m_pTextureCom;

    // Temp. 승한이형 보고 지울 것. Json 사용법
private:
    string  m_szTextureName = "A";
    _int    m_iPitch = 5;

public:
    void Free();
};

END