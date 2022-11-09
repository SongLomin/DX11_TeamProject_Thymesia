#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
struct MESH_DATA;

class ENGINE_DLL CVIBuffer_Model_Instance :
    public CVIBuffer
{
    GAMECLASS_H(CVIBuffer_Model_Instance)
        SHALLOW_COPY(CVIBuffer_Model_Instance)
        CLONE_H(CVIBuffer_Model_Instance, CComponent)

public:
    _uint Get_InstanceCount() const { return m_iNumInstance; }
    _uint Get_MaterialIndex() const {
        return m_iMaterialIndex;
    }

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void	Start() override;

public:
    void Init_NoAnimInstance(const char* In_szModelName, const MEMORY_TYPE In_eModelMemoryType, const _uint In_iNoAnimIndex = 0);
    void Init_Particle(const _uint In_Size);

    virtual HRESULT Render() override;
    void Update(_float fTimeDelta);
    void Update(const vector<PARTICLE_DESC>& In_ParticleDescs);

private:
    ComPtr<ID3D11Buffer>        m_pVBInstance;
    _uint						m_iMaterialIndex = 0;
    _uint						m_iInstanceStride = 0;
    _uint						m_iNumInstance = 0;

    string						m_szName;
    weak_ptr<NoAnim_DATA>			m_pNoAnimData;

private:
    void Free();

};

END