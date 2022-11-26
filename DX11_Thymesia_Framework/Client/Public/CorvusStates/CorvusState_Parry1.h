#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_Parry1 :
    public CCorvusStateBase
{

    GAMECLASS_H(CCorvusState_Parry1);
    SHALLOW_COPY(CCorvusState_Parry1)
        CLONE_H(CCorvusState_Parry1, CComponent)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void Call_AnimationEnd();
    void Play_AttackWithIndex(const _tchar& In_iAttackIndex);

private:
    void    Attack();
    void    Check_InputNextParry();
    void    Update_ParryType();

private:
    _bool m_IsNextParry = false;
    _float m_fDebugAnimationSpeed = 1.f;
    _int m_iEndAttackEffectIndex = -1;

protected: //Parrying Variable;
    PARRY_TYPE              m_eParryType = PARRY_TYPE::PARRY_TYPE_END;
    _bool                   m_bParryed = false;

protected://Parrying Functions
    PARRY_TYPE              Get_ParryType() { return m_eParryType; }
    void                    Success_Parry() { m_bParryed = true; }



   

protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

protected:
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;
    virtual void OnEventMessage(_uint iArg) override;
    virtual void OnDestroy() override;
    void Free();

    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;
    _bool Check_RequirementNextParryState();
    _bool Check_RuquireMnetFirstParryState();
};

END
