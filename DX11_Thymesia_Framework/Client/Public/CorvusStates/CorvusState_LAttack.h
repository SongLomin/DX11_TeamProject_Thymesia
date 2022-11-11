#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_LAttack :
    public CCorvusStateBase
{

    GAMECLASS_H(CCorvusState_LAttack);
    SHALLOW_COPY(CCorvusState_LAttack)
    CLONE_H(CCorvusState_LAttack, CComponent)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void Call_AnimationEnd();
    void Play_AttackWithIndex(const _tchar& In_iAttackIndex);
    void Set_AnimationIndex(_uint iAttackIndex)
    {
        if (m_iAttackIndex == iAttackIndex)
            return;

        m_iAttackIndex = iAttackIndex;
    }
private:
    void Attack();
    void Check_InputNextAttack();
   

private:
    _uint m_iAttackIndex = 7;
    _bool m_IsNextAttack = false;

    _float m_fDebugAnimationSpeed = 1.f;

    _int m_iEndAttackEffectIndex = -1;

protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

protected:
    virtual void OnEventMessage(_uint iArg) override;
    virtual void OnDestroy() override;
     void Free();

    // CLuxiyaStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;
    _bool Check_RequirementNextAttackState();
    _bool Check_RuquireMnetFirstAttackState();
};

END