#pragma once
#include "Base.h"

BEGIN(Engine)

class COptimization_Manager :
    public CBase
{
    DECLARE_SINGLETON(COptimization_Manager)

private:
    struct PERFORMANCE_DESC
    {
        string                              szName;
        chrono::steady_clock::time_point    LastBeginTime{};
        chrono::steady_clock::time_point    LastEndTime{};
        _time                               LastTime{};
        _time                               MinTime{};
        _time                               AvgTime{};
        _time                               AccTime{};
        _time                               MaxTime{};
        _ulonglong                          CallCount{};
    };

public:
    void Begin_PerformanceTime(const string& In_szTag);
    void End_PerformanceTime(const string& In_szTag);
    _time Get_PerfromanceTime(const string& In_szTag);
    void Write_Log();
    void Clear_Logs();


private:
    unordered_map<_hashcode, PERFORMANCE_DESC> m_PerfromanceDescs;

private:
    void Free();

};

END