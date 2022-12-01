#include "stdafx.h"
#include "CorvusStates/Talent_Effect_AvoidThrustLV1.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_AvoidThrustLV1);
CLONE_C(CTalent_Effect_AvoidThrustLV1, CComponent);

_flag CTalent_Effect_AvoidThrustLV1::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::AVOID_THRUST_LV1;
}

void CTalent_Effect_AvoidThrustLV1::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_AvoidThrustLV1::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_AvoidThrustLV1::Free()
{
}
