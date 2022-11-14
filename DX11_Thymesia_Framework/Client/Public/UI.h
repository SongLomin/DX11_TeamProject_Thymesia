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

class CUI :
    public CGameObject
{
    GAMECLASS_H(CUI)
    CLONE_H(CUI, CGameObject)

public:
    struct UI_DESC
    {
        _float fX;
        _float fY;
        _float fSizeX;
        _float fSizeY;
        _float fDepth = 1.f;
    };

public:
    UI_DESC Get_UIDESC() const {
        return m_tUIDesc;
    }

public:
    // CGameObject을(를) 통해 상속됨
    virtual _float Get_CamDistance() override;
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    

public:
    void Set_TextureIndex(const _uint& Index) { m_iTextureIndex = Index; }
    void Set_Texture(const _char* sKey);
    virtual void Set_UIPosition(const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
    virtual void Set_UIPosition(const _float& fX, const _float& fY);
    virtual void Set_UIDesc(UI_DESC _tUIDesc);
    virtual void Set_Size(const _float& fSizeX, const _float& fSizeY);

    void Set_Depth(_float _fDepth);

public: /* Shaking */
    virtual void Add_Shaking(const _float& In_ShakeTime, const _float& _fShakePower = 5.f);

protected:
    virtual HRESULT SetUp_ShaderResource();

private:
    virtual void Update_Shaking(_float fTimeDelta);

protected:
    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;
    weak_ptr<CTexture> m_pTextureCom;
    weak_ptr<CVIBuffer_Rect> m_pVIBufferCom;
    weak_ptr<CFader> m_pFaderCom;


protected:
    FDelegate<>     CallBack_ShakingStart;
    FDelegate<>     CallBack_ShakingEnd;


protected:
    UI_DESC         m_tUIDesc;
    _float4x4		m_ProjMatrix;
    RENDERGROUP     m_eRenderGroup = RENDERGROUP::RENDER_UI;
    _uint           m_iTextureIndex = 0;
    _uint           m_iPassIndex = 0;


protected: /* For. UI Shaking */
    _float          m_fCurrentShakeTime = 0.f;
    _float          m_fShakeFreq = 0.04f;
    _float          m_fCurrentFreq = 0.f;
    _float          m_fPower = 5.f;
    _float2         m_fOffsetPosition{0.f, 0.f};
    _float2         m_fShakedPos = { 0.f,0.f };
    _bool           m_bShaking;

protected:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

protected:
    virtual void OnEventMessage(_uint iArg) override;
    void Free();

};

END