#include "stdafx.h"
#include "CorvusStates/Talent_Effect_NorSwordLV1.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_NorSwordLV1);
CLONE_C(CTalent_Effect_NorSwordLV1, CComponent);


_flag CTalent_Effect_NorSwordLV1::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return 0;
}

void CTalent_Effect_NorSwordLV1::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_NorSwordLV1::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_NorSwordLV1::Free()
{
}
