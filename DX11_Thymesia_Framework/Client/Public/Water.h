#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Ground;
class CPhysXCollider;
END

BEGIN(Client)

class CWater final :
    public CGameObject
{
    GAMECLASS_H(CWater)
    CLONE_H(CWater, CGameObject)

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    void Add_WaterWave(const WATERWAVE_DESC& In_WaterDesc);

public:
    virtual void Load_FromJson(const json& In_Json) override;

private:
    HRESULT SetUp_ShaderResource();

private:
	weak_ptr<CShader>               m_pShaderCom;
	weak_ptr<CRenderer>             m_pRendererCom;
	weak_ptr<CVIBuffer_Ground>      m_pVIBufferCom;

    weak_ptr<CTexture>              m_pNoiseTextureCom;

    _float2                         m_vNoiseUV    = {0.f,0.f};

    vector<WATERWAVE_DESC>          m_WaterWaveDescs;
    _uint                   m_iDescCount = 0;

    _float                  m_fTimeAcc = 0.f;
    _float                  m_fTimeInterval = 0.05f;
    _uint                   m_iPowCount = 0.f;

public:
    void Free();
};

END