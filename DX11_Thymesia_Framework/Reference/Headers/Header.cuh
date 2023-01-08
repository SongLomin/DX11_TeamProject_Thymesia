#pragma once
#include "Cuda_runtime.h"
#include "device_launch_parameters.h"

#ifdef __cplusplus 
extern "C" {//<-- extern ½ÃÀÛ
#endif
	class CudaTest
	{
	public:
		CudaTest(void);
		virtual ~CudaTest(void);
		int sum_cuda(int a, int b, int* c);
	};

#ifdef __cplusplus
}
#endif 