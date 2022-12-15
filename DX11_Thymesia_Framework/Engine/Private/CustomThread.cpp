#include "CustomThread.h"

void CCustomThread::Initialize(void* pArg)
{
}

void CCustomThread::Set_Enable(const _bool In_bEnable)
{
	m_bEnable = In_bEnable;
}


shared_ptr<CCustomThread> CCustomThread::Create(void* pArg)
{
	shared_ptr<CCustomThread> pInstance = shared_ptr<CCustomThread>();

	pInstance->Initialize(pArg);

	return pInstance;
}
