#pragma once
#include "Talent.h"


BEGIN(Engine)

END

BEGIN(Client)

class CPlayer;


class CTalent_Sword :
    public CTalent
{
    GAMECLASS_H(CTalent_Sword)
        CLONE_H(CTalent_Sword, CGameObject)
public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;

    virtual void   TestTalentCheck();

private:
    weak_ptr<CTalent> pSwordLV2 = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
    weak_ptr<CTalent> pAvoidSlashLV1 = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
    weak_ptr<CTalent> pAvoidSlashLV2 = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
    weak_ptr<CTalent> pAvoidThrustLV1 = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
    weak_ptr<CTalent> pAvoidThrustLV2 = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
    
private:
    weak_ptr<CPlayer>       m_pPlayer;
private:
    void Free();

private:
    virtual void    Add_Child(weak_ptr<CUI> pChild) { };
    
};


END