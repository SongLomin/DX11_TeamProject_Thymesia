#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_DynamicCube;
class CShader;
class CRenderer;
END

BEGIN(Client)

class CProp;

class CEditLights final :
    public CGameObject
{
    GAMECLASS_H(CEditLights)
    CLONE_H(CEditLights, CGameObject)
    SHALLOW_COPY(CEditLights)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    virtual _bool IsPicking(const RAY& In_Ray, _float& Out_fRange) override;
    virtual void  OnEventMessage(_uint iArg) override;

private:
    void    View_AddLight();
    void    View_SettingFrog();


private:
    HRESULT SetUp_ShaderResource();

    virtual void Write_Json(json& Out_Json) override;

private:
    typedef vector<string>                      RESOURCE_LIST;

    _bool               m_bSubDraw            = false;
                                              
    _int                m_iPickingIndex       = 0;
    _uint               m_iOption             = 0;
    _float4x4           m_PickingMatrix;

    string              m_szSelectModelName   = "";
    _bool               m_bChangModel         = false;

    RESOURCE_LIST       m_ModelList;

    weak_ptr<CVIBuffer_DynamicCube>     m_pVIBufferCom;
    weak_ptr<CShader>                   m_pShaderCom;
    weak_ptr<CRenderer>                 m_pRendererCom;

    _float4		m_vFogColor = { 1.f, 1.f, 1.f, 1.f };
    _float		m_fFogRange = 30.f;

public:
    void Free();
};

END