#pragma once
#include "LuxiyaStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CLuxiyaState_QTE :
    public CLuxiyaStateBase
{

    GAMECLASS_H(CLuxiyaState_QTE);
    SHALLOW_COPY(CLuxiyaState_QTE)
        CLONE_H(CLuxiyaState_QTE, CComponent)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void Call_AnimationEnd();

private:
    void Attack();

private:
    _uint m_iAttackIndex = 0;
    _bool m_IsNextAttack = false;

    _float m_fDebugAnimationSpeed = 1.f;

    _int m_iEndAttackEffectIndex = -1;

protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

protected:
    virtual void OnDestroy() override;
    virtual void Free() override;

    // CLuxiyaStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;
};

END