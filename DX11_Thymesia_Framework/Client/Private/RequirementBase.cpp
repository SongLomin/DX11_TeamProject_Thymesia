#include "stdafx.h"
#include "RequirementBase.h"


CRequirementBase::CRequirementBase()
{
	Initialize();
}

CRequirementBase::CRequirementBase(void* pArg)
{
	Initialize(pArg);
}

HRESULT CRequirementBase::Initialize(void* pArg)
{
	return S_OK;
}

CRequirementBase::~CRequirementBase()
{
	Free();
}
