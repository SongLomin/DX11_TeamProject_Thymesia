#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_AVoid :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_AVoid);
    CLONE_H(CCorvusState_AVoid, CComponent)
        SHALLOW_COPY(CCorvusState_AVoid)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;


private:
    void Check_InputAgainAvoid();

protected:
    virtual void OnDisable() override;

    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

public:
    void Call_AnimationEnd();
    void Play_AttackWithIndex(const _tchar& In_iAttackIndex);

private:
    _uint m_iDustEffectIndex;
    _uint m_iAvoidIndex;

    _float m_fDebugAnimationSpeed = 1.f;

    _bool m_IsAgainAvoid = false;
protected:
    void Free();

    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;
    _bool Check_RuquireMnetRepeatAvoidkState();
 

};

END
