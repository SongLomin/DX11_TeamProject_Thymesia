#include "stdafx.h"
#include "CorvusStates/Talent_Effect_Long_ClawLv1.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_Long_ClawLv1);
CLONE_C(CTalent_Effect_Long_ClawLv1, CComponent);


_flag CTalent_Effect_Long_ClawLv1::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::LONG_CLAW_LV1;
}

void CTalent_Effect_Long_ClawLv1::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Long_ClawLv1::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Long_ClawLv1::Free()
{
}
