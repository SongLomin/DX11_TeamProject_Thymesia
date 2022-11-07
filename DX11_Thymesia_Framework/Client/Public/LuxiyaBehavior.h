#pragma once
#include "PlayerBehavior.h"

BEGIN(Engine)
class CTransform;
class CModel;
END

BEGIN(Client)

class CLuxiyaBehavior :
    public CPlayerBehavior
{
    GAMECLASS_H(CLuxiyaBehavior);
    CLONE_H(CLuxiyaBehavior, CComponent);

public:
    CLuxiyaBehavior(const CLuxiyaBehavior& rhs);

    // CBehaviorBase을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    
    // CBehaviorBase을(를) 통해 상속됨
    virtual void Stand_Tick(_float fTimeDelta) override;
    virtual void RunStart_Tick(_float fTimeDelta) override;
    virtual void RunStartEnd_Tick(_float fTimeDelta) override;
    virtual void Run_Tick(_float fTimeDelta) override;
    virtual void Stop_Tick(_float fTimeDelta) override;
    virtual void Attack_Tick(_float fTimeDelta) override;
    virtual void LobbyStand_Tick(_float fTimeDelta) override;
    virtual void Dash_Tick(_float fTimeDelta) override;

    virtual _bool Stand_To() override;
    virtual _bool RunStart_To() override;
    virtual _bool RunStartEnd_To() override;
    virtual _bool Run_To() override;
    virtual _bool Stop_To() override;
    virtual _bool Attack_To() override;
    virtual void LobbyStand_To() override;
    virtual _bool Dash_To() override;

private:
    _bool Check_AndChangeAttackState();
    _bool Check_AndChangeDashState();
    _bool Check_AndChangeRunState();
    _bool Check_AndChangeNextAttack();


private:
    _bool m_bRunStartAway = false;

private: /* For Attack */
    _bool                   m_bNextAttack = false;
    _uint                   m_iAttackCnt = 0;
    _uint                   m_iCurrentAttackIndex = 0;
    

protected:
    virtual void OnDestroy() override;
    virtual void Free() override;

    
};

END
