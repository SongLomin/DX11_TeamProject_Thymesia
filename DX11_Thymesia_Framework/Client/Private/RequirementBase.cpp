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

CRequirementBase::~CRequirementBase()
{
	Free();
}
