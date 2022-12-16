#pragma once
#include "CorvusStates/Talent_Effect.h"

BEGIN(Engine)

END

BEGIN(Client)

class CTalent_Effect_Abundant_FlyingLV1 :
    public CTalent_Effect
{
    GAMECLASS_H(CTalent_Effect_Abundant_FlyingLV1);
    CLONE_H(CTalent_Effect_Abundant_FlyingLV1, CComponent);


public:
    virtual _flag Check_Requirement(weak_ptr<CPlayer> pPlayer) override;   
    virtual void Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer) override;
    virtual void UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer) override;

private:
    void Free();

};


END