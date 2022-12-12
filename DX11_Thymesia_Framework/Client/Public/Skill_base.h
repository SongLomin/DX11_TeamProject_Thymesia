#pragma once
#include "ClientComponent.h"

BEGIN(Client)


class CSkill_base :
    public CClientComponent
{
public:
    GAMECLASS_H(CSkill_base)
    CLONE_H(CSkill_base, CComponent)

private:
    _float              m_fSkillCoolTime;
    

    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;

    virtual HRESULT Initialize(void* pArg) override;

};

END
