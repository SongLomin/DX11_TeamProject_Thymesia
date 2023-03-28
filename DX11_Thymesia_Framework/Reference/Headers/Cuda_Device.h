#pragma once
#include "Base.h"

BEGIN(Engine)

class CCuda_Device :
    public CBase
{
    DECLARE_SINGLETON(CCuda_Device)

public:
    CUcontext* Get_CudaContext();
    _bool Is_Valid() const {
        return m_pCudaContext != nullptr;
    }

public:
    HRESULT Ready_Cuda_Device();


private:
    CUcontext m_pCudaContext = nullptr;


private:
    void Free();
};

END