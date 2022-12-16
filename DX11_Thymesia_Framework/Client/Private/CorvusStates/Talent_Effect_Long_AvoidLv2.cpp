#include "stdafx.h"
#include "CorvusStates/Talent_Effect_Long_AvoidLv2.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_Long_AvoidLv2);
CLONE_C(CTalent_Effect_Long_AvoidLv2, CComponent);


_flag CTalent_Effect_Long_AvoidLv2::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::LONG_AVOID_LV2;
}

void CTalent_Effect_Long_AvoidLv2::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Long_AvoidLv2::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Long_AvoidLv2::Free()
{
}
