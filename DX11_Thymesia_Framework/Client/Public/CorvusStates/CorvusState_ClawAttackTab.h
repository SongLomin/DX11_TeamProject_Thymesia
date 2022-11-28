#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_ClawAttackTab :
    public CCorvusStateBase
{

    GAMECLASS_H(CCorvusState_ClawAttackTab);
    SHALLOW_COPY(CCorvusState_ClawAttackTab)
        CLONE_H(CCorvusState_ClawAttackTab, CComponent)

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


private:
    _bool m_IsNextAttack = false;

    _float m_fDebugAnimationSpeed = 1.f;

    _int m_iEndAttackEffectIndex = -1;

    // TODO : Delete

    _bool bTrigger = false;

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