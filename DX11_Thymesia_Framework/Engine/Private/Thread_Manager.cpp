#include "Thread_Manager.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CThread_Manager)




void CThread_Manager::Initialize(const _uint In_iNumLayer)
{
}

void CThread_Manager::Bind_EngineThread(const THREAD_TYPE In_eThread_Type, function<void(_float)> Function, _float fTimeDelta)
{
	m_EngineThreads[In_eThread_Type].push_back(async(launch::async, Function, fTimeDelta));

	//m_EngineThreads[In_eThread_Type].push_back(std::async(std::launch::async, [] { cout << "A" << endl; }));
}

void CThread_Manager::Wait_EngineThread(const THREAD_TYPE In_eThread_Type)
{
	for (auto& elem : m_EngineThreads[In_eThread_Type])
	{
		elem.get();
	}

	Clear_EngineThreads(In_eThread_Type);

	//m_EngineThreads[In_eThread_Type][0] = std::async();

}

void CThread_Manager::Clear_EngineThreads(const THREAD_TYPE In_eThread_Type)
{
	m_EngineThreads[In_eThread_Type].clear();

}

void CThread_Manager::Free()
{
}
