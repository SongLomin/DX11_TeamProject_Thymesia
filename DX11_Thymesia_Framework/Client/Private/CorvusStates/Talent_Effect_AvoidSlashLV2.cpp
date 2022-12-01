#include "stdafx.h"
#include "CorvusStates/Talent_Effect_AvoidSlashLV2.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_AvoidSlashLV2);
CLONE_C(CTalent_Effect_AvoidSlashLV2, CComponent);

_flag CTalent_Effect_AvoidSlashLV2::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::AVOID_SLASH_LV2;
}

void CTalent_Effect_AvoidSlashLV2::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_AvoidSlashLV2::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_AvoidSlashLV2::Free()
{
}
