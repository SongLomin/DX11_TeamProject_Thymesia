#include "Optimization_Manager.h"

IMPLEMENT_SINGLETON(COptimization_Manager)

void COptimization_Manager::Begin_PerformanceTime(const string& In_szTag)
{
#ifdef _USE_PERFROMANCE_LOG_
	_hashcode hashFromTag = hash<string>()(In_szTag);

	auto iter = m_PerfromanceDescs.find(hashFromTag);
	PERFORMANCE_DESC* pElem = nullptr;
	

	if (iter == m_PerfromanceDescs.end())
	{
		m_PerfromanceDescs.emplace(hashFromTag, PERFORMANCE_DESC());
		pElem = &m_PerfromanceDescs[hashFromTag];
		pElem->szName = In_szTag;
#undef max
#undef min
		pElem->MinTime = numeric_limits<_time>::max();
		pElem->MaxTime = numeric_limits<_time>::min();
	}
	else
	{
		pElem = &(*iter).second;
	}

	++pElem->CallCount;

	pElem->LastBeginTime = std::chrono::high_resolution_clock::now();
#endif // _USE_PERFROMANCE_LOG_
}

void COptimization_Manager::End_PerformanceTime(const string& In_szTag)
{
#ifdef _USE_PERFROMANCE_LOG_
	_hashcode hashFromTag = hash<string>()(In_szTag);

	auto iter = m_PerfromanceDescs.find(hashFromTag);

	// Wrong Tag.
	if (m_PerfromanceDescs.end() == iter)
	{
		DEBUG_ASSERT;
	}

	PERFORMANCE_DESC* pElem = &(*iter).second;

	pElem->LastEndTime = std::chrono::high_resolution_clock::now();
	pElem->LastTime = std::chrono::duration_cast<std::chrono::nanoseconds>(pElem->LastEndTime - pElem->LastBeginTime).count();

	pElem->AccTime += pElem->LastTime;

	if (pElem->LastTime < pElem->MinTime)
	{
		pElem->MinTime = pElem->LastTime;
	}

	if (pElem->LastTime > pElem->MaxTime)
	{
		pElem->MaxTime = pElem->LastTime;
	}

	pElem->AvgTime = pElem->AccTime / pElem->CallCount;
#endif // _USE_PERFROMANCE_LOG_
}

_time COptimization_Manager::Get_PerfromanceTime(const string& In_szTag)
{
#ifdef _USE_PERFROMANCE_LOG_
	_hashcode hashFromTag = hash<string>()(In_szTag);

	auto iter = m_PerfromanceDescs.find(hashFromTag);

	// Wrong Tag.
	if (m_PerfromanceDescs.end() == iter)
	{
		DEBUG_ASSERT;
	}

	return (*iter).second.LastTime;
#endif // _USE_PERFROMANCE_LOG_
}

void COptimization_Manager::Write_Log()
{
#ifdef _USE_PERFROMANCE_LOG_

	if (m_PerfromanceDescs.empty())
		return;

	_time CurrentTime = time(nullptr);

	string Path = "..\\Bin\\PerfromanceLogs\\PerfromanceLog" + to_string(CurrentTime) + ".txt";

	ofstream fout(Path);

	if (!fout.is_open())
	{
		DEBUG_ASSERT;
		return;
	}

	fout << u8"██████╗░███████╗██████╗░███████╗██████╗░░█████╗░███╗░░░███╗░█████╗░███╗░░██╗░█████╗░███████╗" << endl;
	fout << u8"██╔══██╗██╔════╝██╔══██╗██╔════╝██╔══██╗██╔══██╗████╗░████║██╔══██╗████╗░██║██╔══██╗██╔════╝" << endl;
	fout << u8"██████╔╝█████╗░░██████╔╝█████╗░░██████╔╝██║░░██║██╔████╔██║███████║██╔██╗██║██║░░╚═╝█████╗░░" << endl;
	fout << u8"██╔═══╝░██╔══╝░░██╔══██╗██╔══╝░░██╔══██╗██║░░██║██║╚██╔╝██║██╔══██║██║╚████║██║░░██╗██╔══╝░░" << endl;
	fout << u8"██║░░░░░███████╗██║░░██║██║░░░░░██║░░██║╚█████╔╝██║░╚═╝░██║██║░░██║██║░╚███║╚█████╔╝███████╗" << endl;
	fout << u8"╚═╝░░░░░╚══════╝╚═╝░░╚═╝╚═╝░░░░░╚═╝░░╚═╝░╚════╝░╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░╚══╝░╚════╝░╚══════╝" << endl << endl;
	fout << u8"░█████╗░██╗░░██╗███████╗░█████╗░██╗░░██╗███████╗██████╗░" << endl;
	fout << u8"██╔══██╗██║░░██║██╔════╝██╔══██╗██║░██╔╝██╔════╝██╔══██╗" << endl;
	fout << u8"██║░░╚═╝███████║█████╗░░██║░░╚═╝█████═╝░█████╗░░██████╔╝" << endl;
	fout << u8"██║░░██╗██╔══██║██╔══╝░░██║░░██╗██╔═██╗░██╔══╝░░██╔══██╗" << endl;
	fout << u8"╚█████╔╝██║░░██║███████╗╚█████╔╝██║░╚██╗███████╗██║░░██║" << endl;
	fout << u8"░╚════╝░╚═╝░░╚═╝╚══════╝░╚════╝░╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝" << endl;

	fout << "__________________________________________________" << endl;
	fout << "For parts that require performance checks, " << endl;
	fout << "you can record them using GAMEINSTANCE's 'Begin_PerformanceTime' and 'End_PerformanceTime' functions." << endl;
	fout << "__________________________________________________" << endl;

	vector<pair<_hashcode, PERFORMANCE_DESC>> vec(m_PerfromanceDescs.begin(), m_PerfromanceDescs.end());

	sort(vec.begin(), vec.end(),
		[](pair<_hashcode, PERFORMANCE_DESC>& Left, pair<_hashcode, PERFORMANCE_DESC>& Right)
		{
			return Left.second.AvgTime > Right.second.AvgTime;
		});

	_int iIndex = 0;

	for (auto& elem : vec)
	{
		fout << "Resource Rank "<< iIndex++ <<"." << endl;
		fout << "__________________________________________________" << endl;
		fout << "CHECK NAME		: " << elem.second.szName << endl;
		fout << "AVG TIME		: " << elem.second.AvgTime <<endl;
		fout << "MIN TIME		: " << elem.second.MinTime <<endl;
		fout << "MAX TIME		: " << elem.second.MaxTime <<endl;
		fout << "ACC TIME		: " << elem.second.AccTime <<endl;
		fout << "CALL COUNT		: " << elem.second.CallCount <<endl;
		fout << "__________________________________________________" << endl << endl;
		
	}

	fout.close();
#endif // _USE_PERFROMANCE_LOG_
}

void COptimization_Manager::Clear_Logs()
{
	m_PerfromanceDescs.clear();
}

void COptimization_Manager::Free()
{
	Write_Log();
}
