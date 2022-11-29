#pragma once
#include "ClientComponent.h"


BEGIN(Engine)

END

BEGIN(Client)

class CPlayer;

class CTalent_Effect :
    public CClientComponent
{
    GAMECLASS_H(CTalent_Effect)
    CLONE_H(CTalent_Effect, CComponent)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
public:
    virtual _flag Check_Requirement(weak_ptr<CPlayer> pPlayer);

    virtual void Bind_Talent_Effect(weak_ptr<CPlayer> pPlayer) {};
    virtual void UnBind_Talent_Effect(weak_ptr<CPlayer> pPlayer) {};

private:
    void Free();

};


END