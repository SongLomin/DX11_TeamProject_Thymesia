#include "Thread.h"

void CThread::Initialize(void* pArg)
{
}

shared_ptr<CThread> CThread::Create(void* pArg)
{
	shared_ptr<CThread> pInstance = shared_ptr<CThread>();

	pInstance->Initialize(pArg);

	return pInstance;
}
