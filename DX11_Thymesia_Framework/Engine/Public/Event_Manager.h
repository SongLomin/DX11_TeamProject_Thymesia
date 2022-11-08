#pragma once
#include "Base.h"
BEGIN(Engine)

class CEvent_Manager final : public CBase
{
	DECLARE_SINGLETON(CEvent_Manager)

public:
	vector<FDelegate<void*>> m_Events;



public:
	void Free();
};

END