#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instance :
    public CVIBuffer
{
    GAMECLASS_H(CVIBuffer_Point_Instance)
    //SHALLOW_COPY(CVIBuffer_Point_Instance)
    CLONE_H(CVIBuffer_Point_Instance, CComponent)

public:
    CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& rhs);

private:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void	Start() override;

public:
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    void Update(_float fTimeDelta);

private:
    ComPtr<ID3D11Buffer>        m_pVBInstance;
    _uint						m_iInstanceStride = 0;
    _uint						m_iNumInstance = 0;

    vector<_float>              m_pInstanceSpeeds;

private:
    void Free();

};

END