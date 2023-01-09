#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_ClawAttackHold :
    public CCorvusStateBase
{

    GAMECLASS_H(CCorvusState_ClawAttackHold);
    SHALLOW_COPY(CCorvusState_ClawAttackHold)
        CLONE_H(CCorvusState_ClawAttackHold, CComponent)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
   // void Call_AnimationEnd(_uint iEndAnimIndex);
    void Play_AttackWithIndex(const _tchar& In_iAttackIndex);
    void Attack();
    void Check_InputNextAttack();

    void Call_NextKeyFrame(const _uint& In_KeyIndex);


private:
    _bool m_IsNextAttack = false;

    

    _int m_iEndAttackEffectIndex = -1;

    _float m_fDissolveTimeClaw = 0.f;
    _float m_fDissolveTimeArm = 0.f;
    _float m_fDissolveAmountClaw = 1.f;
    _float m_fDissolveAmountArm = 1.f;
    _float3 m_vDissolveDir = { 1.f,0.f,0.f };

    _bool m_bDissolve = false;


private:
    weak_ptr<CAnimation> m_pThisAnimationCom;

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