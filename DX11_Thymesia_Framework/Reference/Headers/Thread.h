#pragma once
#include "Engine_Defines.h"

template <class... ARGS>
class CThread
{


public:
	
	
private:
	void Initialize(void* pArg);

public:
	static shared_ptr<CThread> Create(void* pArg = nullptr);


private:
	std::function<void(ARGS...)> Function;

private:
	_bool	m_bDead = false;

};

