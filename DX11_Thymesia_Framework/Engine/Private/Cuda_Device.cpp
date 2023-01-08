#include "Cuda_Device.h"

IMPLEMENT_SINGLETON(CCuda_Device);

CUcontext* CCuda_Device::Get_CudaContext()
{
	return &m_pCudaContext;
}

HRESULT CCuda_Device::Ready_Cuda_Device()
{
	cuInit(0);

	CUresult result = cuCtxCreate(&m_pCudaContext, 0, 0);

	return S_OK;
}

void CCuda_Device::Free()
{
	cuCtxDestroy(m_pCudaContext);
}
