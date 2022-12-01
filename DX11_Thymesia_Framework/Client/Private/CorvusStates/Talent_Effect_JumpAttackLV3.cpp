#include "stdafx.h"
#include "CorvusStates/Talent_Effect_JumpAttackLV3.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_JumpAttackLV3);
CLONE_C(CTalent_Effect_JumpAttackLV3, CComponent);


_flag CTalent_Effect_JumpAttackLV3::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return (_flag)TALENT_EFFECT_FLAG::JUMP_ATTACK_LV3;
}

void CTalent_Effect_JumpAttackLV3::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_JumpAttackLV3::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_JumpAttackLV3::Free()
{
}
