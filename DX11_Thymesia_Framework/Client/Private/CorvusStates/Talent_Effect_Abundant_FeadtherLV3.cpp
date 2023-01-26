#include "stdafx.h"
#include "CorvusStates/Talent_Effect_Abundant_FeadtherLV3.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_Abundant_FlyingLV3);
CLONE_C(CTalent_Effect_Abundant_FlyingLV3, CComponent);


_flag CTalent_Effect_Abundant_FlyingLV3::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::ABUNDANT_FEATHER_LV3;
}

void CTalent_Effect_Abundant_FlyingLV3::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Abundant_FlyingLV3::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Abundant_FlyingLV3::Free()
{
}
