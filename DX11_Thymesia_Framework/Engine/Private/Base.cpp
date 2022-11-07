#include "..\Public\Base.h"

_bool CBase::Set_Enable(_bool _Enable, void* _Arg)
{
	if (_Enable == m_bEnable)
		return false;

	m_bEnable = _Enable;
	m_bEnable ? OnEnable(_Arg) : OnDisable();

	return true;
}

bool CBase::Get_Enable()
{
	return m_bEnable;
}

void CBase::Set_ThisInstance(weak_ptr<CBase> Other)
{
	m_this = Other;
}


void CBase::Free()
{
}
