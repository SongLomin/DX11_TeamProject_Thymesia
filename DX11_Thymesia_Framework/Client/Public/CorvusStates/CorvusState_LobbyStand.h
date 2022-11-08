#pragma once
#include "LuxiyaStateBase.h"
BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CLuxiyaState_LobbyStand :
    public CLuxiyaStateBase
{
    GAMECLASS_H(CLuxiyaState_LobbyStand);
    SHALLOW_COPY(CLuxiyaState_LobbyStand)
    CLONE_H(CLuxiyaState_LobbyStand, CComponent)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

protected:
    virtual void Free() override;


    // CLuxiyaStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;

};

END