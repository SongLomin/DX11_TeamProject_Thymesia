#include "stdafx.h"
#include "CorvusStates/Talent_Effect_JumpAttackLV1.h"
#include "GameManager.h"

GAMECLASS_C(CTalent_Effect_JumpAttackLV1);
CLONE_C(CTalent_Effect_JumpAttackLV1, CComponent);


_flag CTalent_Effect_JumpAttackLV1::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
    return 0;
}

void CTalent_Effect_JumpAttackLV1::Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_JumpAttackLV1::UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer)
{
    return;
}

void CTalent_Effect_JumpAttackLV1::Free()
{
}
