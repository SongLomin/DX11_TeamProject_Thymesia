#pragma once
#include "Engine_Defines.h"

class CCustomThread
{

public:
	void Set_Enable(const _bool In_bEnable);
	
protected:
	void Initialize(void* pArg);

public:
	static shared_ptr<CCustomThread> Create(void* pArg = nullptr);


protected:
	_bool	m_bEnable = false;

};

