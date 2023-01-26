//#include "Update_Particle.cuh"


__device__  PARTICLE_DESC Play_TestInternal(const _uint& i, _float fTimeDelta, _matrix BoneMatrix, PARTICLE_DESC* input)
{
	PARTICLE_DESC desc = *input;
	desc.fCurrentLifeTime = 999.f;

	return desc;
}


__global__ void kernel_UpdateParticle(_float fTimeDelta, _matrix BoneMatrix, PARTICLE_DESC* input, PARTICLE_DESC* output, _int size)
{
	_int i = blockIdx.x * blockDim.x + threadIdx.x;

	if (i < size)
	{
		*output = Play_TestInternal(i, fTimeDelta, BoneMatrix, input);
	}


}

__global__ void Kernel(int size)
{

}

__host__ void CudaMain_UpdateParticle(float fTimeDelta, _matrix BoneMatrix, PARTICLE_DESC* input, PARTICLE_DESC* output, _int size)
{
	
}

__host__ void CudaMain_UpdateParticleTEST(float fTimeDelta)
{
	//PARTICLE_DESC* pInputParticleDescToCuda;
	//PARTICLE_DESC* pOutputParticleDescToCuda;

	////_size_t Size = sizeof(PARTICLE_DESC) * m_tEffectParticleDesc.iMaxInstance;

	//cudaMalloc((void**)&pInputParticleDescToCuda, 100);
	//cudaMalloc((void**)&pOutputParticleDescToCuda, 100);

	////cudaMemcpy(pInputParticleDescToCuda, &m_tParticleDescs[0], Size, cudaMemcpyHostToDevice);

	//cudaFree(pInputParticleDescToCuda);
	//cudaFree(pOutputParticleDescToCuda);
}

