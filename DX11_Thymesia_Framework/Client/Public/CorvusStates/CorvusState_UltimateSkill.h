#pragma once
#include "LuxiyaStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CLuxiya_UltimateUI;
class CLuxiya_UltimateEffect;

class CLuxiyaState_UltimateSkill :
    public CLuxiyaStateBase
{

    GAMECLASS_H(CLuxiyaState_UltimateSkill);
    SHALLOW_COPY(CLuxiyaState_UltimateSkill)
    CLONE_H(CLuxiyaState_UltimateSkill, CComponent)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void Call_AnimationEnd();

    void Call_OnNextKeyFrame(const _uint& In_iKeyIndex);

private:
    void Attack();
    void Divide_Display();

private:
    _uint m_iAttackIndex = 0;
    _bool m_IsNextAttack = false;

    _float m_fDebugAnimationSpeed = 1.f;

    _int m_iEndAttackEffectIndex = -1;

    _uint m_iCurrentDivideMaskIndex = 3;

private:
    list<weak_ptr<CLuxiya_UltimateUI>> m_pLuxiyaUltimateUIs;
    list<weak_ptr<CLuxiya_UltimateEffect>> m_pLuxiyaUltimateEffects;

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