#include "stdafx.h"
#include "RequirementChecker.h"

GAMECLASS_C(CRequirementChecker)

HRESULT CRequirementChecker::Initialize(void* pArg)
{

	return S_OK;
}

void CRequirementChecker::Add_Requirement(shared_ptr<CRequirementBase> pRequirment)
{
	m_pRequirements.emplace_back(pRequirment);
}

_bool CRequirementChecker::Check_Requirments()
{
	_bool bResult = true;

	if (m_pRequirements.empty())//들어오는 조건 자체가 없다면
	{
		return true;
	}

	for (auto iter = m_pRequirements.begin(); iter != m_pRequirements.end();)
	{
		if (!(*iter))
		{
			iter = m_pRequirements.erase(iter);
		}
		else
		{
			if (!(*iter)->Check_Requirement())
			{
				bResult = false;
				break;
			}
			else
			{
				(*iter)->Callback_OnRequirementTrue();
				(*iter)->Callback_OnRequirementTrue.Clear();
			}
			++iter;
		}
	}
	Update_Vaild();
	
	return bResult;
}

void CRequirementChecker::Clear_Requirements()
{
	m_pRequirements.clear();
}

void CRequirementChecker::Update_Vaild()
{
	for (auto iter = m_pRequirements.begin(); iter != m_pRequirements.end();)
	{
		if (!(*iter))
		{
			iter = m_pRequirements.erase(iter);
		}
		else if(!(*iter)->Is_Valid())
		{
			iter = m_pRequirements.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

shared_ptr<CRequirementChecker> CRequirementChecker::Create(void* pArg)
{
	shared_ptr< CRequirementChecker> pRequireChecker = make_shared<CRequirementChecker>();

	pRequireChecker->Initialize(pArg);

	return pRequireChecker;
}

void CRequirementChecker::Free()
{
}