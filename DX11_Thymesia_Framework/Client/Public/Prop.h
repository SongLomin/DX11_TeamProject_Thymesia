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
    virtual void Thread_PreLateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

protected:
    virtual HRESULT SetUp_ShaderResource();
    void            SetUp_Invisibility();

protected:
    weak_ptr<CTexture>  m_pMaskingTextureCom;
    weak_ptr<CModel>    m_pModelCom;
    weak_ptr<CShader>   m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;

protected:
    RENDERGROUP m_eRenderGroup          = RENDERGROUP::RENDER_NONALPHABLEND;
    
    _int        m_iPassIndex            = 0;
    _float      m_fCullingOffsetRange   = 0.f;
    _bool       m_bRendering            = true;
    _bool       m_bInvisibility         = true;

    _float3     m_vCenterOffset{ 0.f, 0.f, 0.f };

    _bool       m_bNearPlayer           = false;//플레이어 근처에 있을 때 아웃라인 활성화
    _bool       m_bUsingInteraction     = false;
    _float      m_fOutLineBlurIntensity = 0.f;//For Interaction Props;

protected:
    void Free();
};

END