#include "stdafx.h"
#include "CorvusStates/Talent_Effect_Long_ClawLv3.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_Long_ClawLv3);
CLONE_C(CTalent_Effect_Long_ClawLv3, CComponent);


_flag CTalent_Effect_Long_ClawLv3::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::LONG_CLAW_LV2;
}

void CTalent_Effect_Long_ClawLv3::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Long_ClawLv3::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Long_ClawLv3::Free()
{
}
