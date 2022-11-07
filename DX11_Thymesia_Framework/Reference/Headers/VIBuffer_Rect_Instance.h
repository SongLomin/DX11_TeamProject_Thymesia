#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance :
    public CVIBuffer
{
    GAMECLASS_H(CVIBuffer_Rect_Instance)
    SHALLOW_COPY(CVIBuffer_Rect_Instance)
    CLONE_H(CVIBuffer_Rect_Instance, CComponent)

public:
    _uint Get_InstanceCount() const { return m_iNumInstance; }

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void	Start() override;

public:
    void Init_Particle(const _uint& In_Size);

    virtual HRESULT Render() override;
    void Update(_float fTimeDelta);
    void Update(const vector<PARTICLE_DESC>& In_ParticleDescs);

private:
    ComPtr<ID3D11Buffer>        m_pVBInstance;
    _uint						m_iInstanceStride = 0;
    _uint						m_iNumInstance = 0;

    shared_ptr<_float[]>        m_pInstanceSpeeds;

private:
    virtual void Free() override;

};

END