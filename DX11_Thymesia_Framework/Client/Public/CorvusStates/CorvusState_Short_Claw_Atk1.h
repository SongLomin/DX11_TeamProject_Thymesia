#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_Short_Claw_Atk1 :
    public CCorvusStateBase
{

    GAMECLASS_H(CCorvusState_Short_Claw_Atk1);
    SHALLOW_COPY(CCorvusState_Short_Claw_Atk1)
        CLONE_H(CCorvusState_Short_Claw_Atk1, CComponent)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;


public:
    void Call_AnimationEnd(_uint iEndAnimIndex);
    void Call_NextAnimationKey(const _uint& In_iKeyIndex);
    //void Play_AttackWithIndex(const _tchar& In_iAttackIndex);
    void Attack();
    void Check_InputNextAttack();


private:
    _bool m_IsNextAttack = false;

    

    _int m_iEndAttackEffectIndex = -1;

    // TODO : Delete

    _float m_fDissolveTimeClaw = 0.f;
    _float m_fDissolveTimeArm = 0.f;
    _float m_fDissolveAmountClaw = 0.f;
    _float m_fDissolveAmountArm = 0.f;
    _float3 m_vDissolveDir = { 0.f,0.f,0.f };

    _bool bTrigger = false;

    _bool m_bDissolve = false;
    _bool m_bDissolveAppear = false;

    
    weak_ptr<CAnimation> m_ThisStateAnimationCom;

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