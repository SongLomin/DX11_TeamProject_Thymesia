#pragma once
#include "LuxiyaStateBase.h"
BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CLuxiyaState_Win :
    public CLuxiyaStateBase
{
    GAMECLASS_H(CLuxiyaState_Win)
    CLONE_H(CLuxiyaState_Win, CComponent)
    SHALLOW_COPY(CLuxiyaState_Win)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void Call_AnimationEnd();
    void Call_OnNextKeyFrame(const _uint& In_iKeyIndex);

protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;
    virtual _bool Check_AndChangeNextState() override;

private:
    _bool       m_bAnimEnd = false;

protected:
    virtual void Free() override;

};

END