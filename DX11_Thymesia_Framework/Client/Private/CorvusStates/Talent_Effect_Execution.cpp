#include "stdafx.h"
#include "CorvusStates/Talent_Effect_Execution.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_Execution);
CLONE_C(CTalent_Effect_Execution, CComponent);


_flag CTalent_Effect_Execution::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::EXECUTION_LV1;
}

void CTalent_Effect_Execution::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Execution::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Execution::Free()
{
}
