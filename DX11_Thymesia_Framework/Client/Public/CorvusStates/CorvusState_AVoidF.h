#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_AVoidF :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_AVoidF);
    CLONE_H(CCorvusState_AVoidF, CComponent)
        SHALLOW_COPY(CCorvusState_AVoidF)

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

    _float m_fDebugAnimationSpeed = 1.f;

    _bool m_IsAgainAvoid = false;

    _float m_fNextAvoidRatio = 0.46f;

protected:
    void Free();
    virtual void OnDestroy() override;
    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;
    _bool Check_RuquireMnetRepeatAvoidkState();
 

};

END
