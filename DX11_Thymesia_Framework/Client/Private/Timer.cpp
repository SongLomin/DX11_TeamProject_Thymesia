#include "stdafx.h"
#include "Timer.h"


GAMECLASS_C(CTimer);
CLONE_C(CTimer, CComponent)


HRESULT CTimer::Initialize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CTimer::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

shared_ptr<CComponent> CTimer::Clone(_uint iLevelIndex, void* pArg)
{
    return shared_ptr<CComponent>();
}
