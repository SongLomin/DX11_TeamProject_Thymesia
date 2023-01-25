#pragma once

#include <cuda_runtime.h>
#include "Client_Defines.h"



__device__  PARTICLE_DESC Play_TestInternal(const _uint& i, _float fTimeDelta, _matrix BoneMatrix, PARTICLE_DESC* input);
__global__ void kernel_UpdateParticle(_float fTimeDelta, _matrix BoneMatrix, PARTICLE_DESC* input, PARTICLE_DESC* output, _int size);
__global__ void Kernel(int size);

__host__ void		CudaMain_UpdateParticle(float fTimeDelta, _matrix BoneMatrix, PARTICLE_DESC* input, PARTICLE_DESC* output, _int size);
__host__ void		CudaMain_UpdateParticleTEST(float fTimeDelta);