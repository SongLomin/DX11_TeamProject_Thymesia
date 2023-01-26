#include "stdafx.h"
#include "Requirement_Time.h"
#include "GameInstance.h"

_bool CRequirement_Time::Check_Requirement()
{
	return !(m_fReqTime > 0.f);
}

_bool CRequirement_Time::Is_Valid()
{
	m_fReqTime -= GAMEINSTANCE->Get_DeltaTime();
	if (m_fReqTime < 0.f)
	{ 
		m_fReqTime = 0.f;
	
		return false;
	}
	return true;
}

void CRequirement_Time::Init_Req(const _float In_fReqTime)
{
	m_fReqTime = In_fReqTime;
	m_fReqOriginTime = m_fReqTime;
}

_float CRequirement_Time::Get_RatioTime()
{
	_float fRatio = 1.f - (m_fReqTime / m_fReqOriginTime);

	fRatio = min(1.f, fRatio);
	
	return fRatio;
}

HRESULT CRequirement_Time::Initialize(void* pArg)
{
	m_fReqTime = 0.f;

	return S_OK;
}

void CRequirement_Time::Free()
{
}
