#include "stdafx.h"
#include "CorvusStates/Talent_Effect_Reckless_ParringLV1.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_Reckless_ParringLV1);
CLONE_C(CTalent_Effect_Reckless_ParringLV1, CComponent);


_flag CTalent_Effect_Reckless_ParringLV1::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::RECKLESS_PARRING_LV1;
}

void CTalent_Effect_Reckless_ParringLV1::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Reckless_ParringLV1::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_Reckless_ParringLV1::Free()
{
}
