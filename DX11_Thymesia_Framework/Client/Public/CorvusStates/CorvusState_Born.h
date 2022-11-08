#pragma once
#include "LuxiyaStateBase.h"
BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CLuxiyaState_Born :
    public CLuxiyaStateBase
{
    GAMECLASS_H(CLuxiyaState_Born)
    CLONE_H(CLuxiyaState_Born, CComponent)
    SHALLOW_COPY(CLuxiyaState_Born)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void Call_AnimationEnd();

protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;
    virtual _bool Check_AndChangeNextState() override;

private:
    _bool       m_bFirst = false;
    _bool       m_bSecond = false;

protected:
    virtual void Free() override;

};

END