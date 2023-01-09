#pragma once

#include <cuda_runtime.h>
#include "Client_Defines.h"



#ifdef __cplusplus 

extern "C" {//<-- extern ½ÃÀÛ

#endif

	__device__  PARTICLE_DESC Play_TestInternal(const _uint& i, _float fTimeDelta, _matrix BoneMatrix, PARTICLE_DESC* input);
	__global__ void kernel_UpdateParticle(_float fTimeDelta, _matrix BoneMatrix, PARTICLE_DESC* input, PARTICLE_DESC* output, _int size);
	__global__ void Kernel(int size);

	void		CudaMain_UpdateParticle(float fTimeDelta, _matrix BoneMatrix, PARTICLE_DESC* input, PARTICLE_DESC* output, _int size);

#ifdef __cplusplus 

}

#endif

