#pragma once
#include "ClientComponent.h"

BEGIN(Client)

class CStateBase;

class CBehaviorBase :
    public CClientComponent
{
public:

public:
    virtual _bool Change_State(weak_ptr<CStateBase> In_pNextState);

public:
    virtual void Stand_Tick(_float fTimeDelta) PURE;
    virtual void RunStart_Tick(_float fTimeDelta) PURE;
    virtual void RunStartEnd_Tick(_float fTimeDelta) PURE;
    virtual void Run_Tick(_float fTimeDelta) PURE;
    virtual void Stop_Tick(_float fTimeDelta) PURE;
    virtual void Attack_Tick(_float fTimeDelta) PURE;
    virtual void LobbyStand_Tick(_float fTimeDelta) PURE;
    virtual void Dash_Tick(_float fTimeDelta) PURE;

    virtual _bool Stand_To() PURE;
    virtual _bool RunStart_To() PURE;
    virtual _bool RunStartEnd_To() PURE;
    virtual _bool Run_To() PURE;
    virtual _bool Stop_To() PURE;
    virtual _bool Attack_To() PURE;
    virtual _bool Dash_To() PURE;
    virtual void LobbyStand_To() PURE;
    

    void State_Tick(_float fTimeDelta);
    void State_LateTick(_float fTimeDelta);

private:
    _bool Change_State_Internal(weak_ptr<CStateBase> In_pNextState);

public:
    virtual void OnEventMessage(_uint iArg) override;

protected:
    _uint m_iTimeScaleLayer = 0;

private:
    weak_ptr<CStateBase> m_pCurrentState;

protected: /* For EditMode */
    _bool m_bEdit = false;
    weak_ptr<CStateBase> m_pEditState;
    
};

END