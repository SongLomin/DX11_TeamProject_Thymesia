#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_ClawAttackAway :
    public CCorvusStateBase
{

    GAMECLASS_H(CCorvusState_ClawAttackAway)
    SHALLOW_COPY(CCorvusState_ClawAttackAway)
    CLONE_H(CCorvusState_ClawAttackAway, CComponent)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void Call_AnimationEnd();
    void Play_AttackWithIndex(const _tchar& In_iAttackIndex);
    void Attack();
    void Check_InputNextAttack();

    void Call_NextKeyFrame(const _uint& In_KeyIndex);

private:
    weak_ptr<CAnimation> m_pThisAnimationCom;

private:
    _bool m_IsNextAttack = false;

    _float m_fDebugAnimationSpeed = 1.f;

    _uint m_iEffectIndex = 0;

protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

protected:
    virtual void OnEventMessage(_uint iArg) override;
    virtual void OnDestroy() override;
    void Free();

    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;
    _bool Check_RequirementNextAttackState();
    _bool Check_RuquireMnetFirstAttackState();
};

END