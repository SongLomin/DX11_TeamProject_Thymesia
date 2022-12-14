#pragma once
#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Triangle_Instance :
    public CVIBuffer
{
    GAMECLASS_H(CVIBuffer_Triangle_Instance)
    SHALLOW_COPY(CVIBuffer_Triangle_Instance)
    CLONE_H(CVIBuffer_Triangle_Instance, CComponent)

public:
    _uint Get_InstanceCount() const { return m_iNumInstance; }

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void	Start() override;

public:
    void Init_Particle(const _uint& In_Size);

    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    void Update(_float fTimeDelta);
    void Update(const vector<PARTICLE_DESC>& In_ParticleDescs);

private:
    ComPtr<ID3D11Buffer>        m_pVBInstance;
    _uint						m_iInstanceStride = 0;
    _uint						m_iNumInstance = 0;

    shared_ptr<_float[]>        m_pInstanceSpeeds;

private:
    void Free();

};

END