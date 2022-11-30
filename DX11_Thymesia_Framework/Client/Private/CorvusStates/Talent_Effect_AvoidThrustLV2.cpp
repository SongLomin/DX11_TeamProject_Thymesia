#include "stdafx.h"
#include "CorvusStates/Talent_Effect_AvoidThrustLV2.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_AvoidThrustLV2);
CLONE_C(CTalent_Effect_AvoidThrustLV2, CComponent);

_flag CTalent_Effect_AvoidThrustLV2::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::AVOID_THRUST_LV2;
}

void CTalent_Effect_AvoidThrustLV2::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_AvoidThrustLV2::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_AvoidThrustLV2::Free()
{
}
