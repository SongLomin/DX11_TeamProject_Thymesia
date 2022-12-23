#include "NvCloth_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CNvCloth_Manager)

Solver* CNvCloth_Manager::Get_Solver()
{
	return m_pSolver;
}

Factory* CNvCloth_Manager::Get_Factory()
{
	return m_pFactory;
}

HRESULT CNvCloth_Manager::Initialize()
{
	m_pGraphicsContextManager = DBG_NEW CCustomContextManagerCallback(DEVICE);

	m_pGraphicsContextManager->mContext = DEVICECONTEXT;

	m_pAllocatorCallback = DBG_NEW CCustomPxAllocatorCallback();
	m_pErrorCallback = DBG_NEW CCustomPxErrorCallback();
	m_pAssertHander = DBG_NEW CCustomPxAssertHandler();

	InitializeNvCloth(m_pAllocatorCallback, m_pErrorCallback, m_pAssertHander, nullptr);

	m_pFactory = NvClothCreateFactoryCPU();
	//m_pFactory = NvClothCreateFactoryDX11(m_pGraphicsContextManager);
	//We need to release all DX objects after destroying the factory.
	
	m_pSolver = m_pFactory->createSolver();

	return S_OK;
}

void CNvCloth_Manager::Tick(_float fTimeDelta)
{
	if (m_pSolver->getSimulationChunkCount() <= 0)
		return;

	if (fTimeDelta > 0.2f)
	{
		return;
	}

	shared_ptr<CThread_Manager> pThread_Manager = GET_SINGLE(CThread_Manager);

	m_pGraphicsContextManager->mDevice = DEVICE;
	m_pGraphicsContextManager->mContext = DEVICECONTEXT;
	m_pGraphicsContextManager->mSynchronizeResources = false;
	m_pSolver->beginSimulation(fTimeDelta);
	for (int i = 0; i < m_pSolver->getSimulationChunkCount(); i++)
	{
		pThread_Manager->Enqueue_Job(bind(&Solver::simulateChunk, m_pSolver, i));
		//m_pSolver->simulateChunk(i);
	} 

	pThread_Manager->Wait_JobDone();

	m_pSolver->endSimulation();

}

void CNvCloth_Manager::OnDestroy()
{
	Safe_Delete(m_pSolver);
	NvClothDestroyFactory(m_pFactory);
	Safe_Delete(m_pGraphicsContextManager);
	Safe_Delete(m_pAllocatorCallback);
	Safe_Delete(m_pErrorCallback);
	Safe_Delete(m_pAssertHander);
	
}

void CNvCloth_Manager::Free()
{
}
