#include "stdafx.h"
#include "CorvusStates/Talent_Effect_Feadther_FlyingLV4.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_Feadther_FlyingLV4);
CLONE_C(CTalent_Effect_Feadther_FlyingLV4, CComponent);


_flag CTalent_Effect_Feadther_FlyingLV4::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::FEATHER_FLYING_LV4;
}

void CTalent_Effect_Feadther_FlyingLV4::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Feadther_FlyingLV4::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Feadther_FlyingLV4::Free()
{
}
