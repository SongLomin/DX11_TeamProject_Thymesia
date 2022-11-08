#pragma once
#include "LuxiyaStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CLuxiyaState_Attack :
    public CPlayerStateBase
{

    GAMECLASS_H(CLuxiyaState_Attack);
    SHALLOW_COPY(CLuxiyaState_Attack)
    CLONE_H(CLuxiyaState_Attack, CComponent)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void Call_AnimationEnd();
    void Play_AttackWithIndex(const _uint& In_iAttackIndex);

private:
    void Attack();
    void Check_InputNextAttack();

private:
    _uint m_iAttackIndex = 0;
    _bool m_IsNextAttack = false;

    _float m_fDebugAnimationSpeed = 1.f;

    _int m_iEndAttackEffectIndex = -1;

protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

protected:
    virtual void OnEventMessage(_uint iArg) override;
    virtual void OnDestroy() override;
    virtual void Free() override;

    // CLuxiyaStateBase��(��) ���� ��ӵ�
    virtual _bool Check_AndChangeNextState() override;
    _bool Check_RequirementNextAttackState();
};

END