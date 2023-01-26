#include "../Default/stdafx.h"
#include "CorvusStates/Talent_Effect_NorSwordLV2.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_NorSwordLV2);
CLONE_C(CTalent_Effect_NorSwordLV2, CComponent);


_flag CTalent_Effect_NorSwordLV2::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::SABER_ATTACK_LV2;
}

void CTalent_Effect_NorSwordLV2::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_NorSwordLV2::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_NorSwordLV2::Free()
{
}
