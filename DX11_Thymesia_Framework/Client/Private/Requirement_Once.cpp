#include "stdafx.h"
#include "Requirement_Once.h"

_bool CRequirement_Once::Check_Requirement()
{
    return !Get_Enable();
}

_bool CRequirement_Once::Is_Valid()
{
    return false;
}

HRESULT CRequirement_Once::Initialize(void* pArg)
{
    return S_OK;
}

void CRequirement_Once::Free()
{

}
