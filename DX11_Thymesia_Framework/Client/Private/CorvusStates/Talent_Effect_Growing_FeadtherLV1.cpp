#include "stdafx.h"
#include "CorvusStates/Talent_Effect_Growing_FeadtherLV1.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_Growing_FlyingLV1);
CLONE_C(CTalent_Effect_Growing_FlyingLV1, CComponent);


_flag CTalent_Effect_Growing_FlyingLV1::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::GROWING_FEATHER_LV1;
}

void CTalent_Effect_Growing_FlyingLV1::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Growing_FlyingLV1::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Growing_FlyingLV1::Free()
{
}
