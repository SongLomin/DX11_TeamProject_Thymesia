#pragma once
#include  "CorvusStates/CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_Varg_Execution:
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_Varg_Execution);
    CLONE_H(CCorvusState_Varg_Execution, CComponent)
        SHALLOW_COPY(CCorvusState_Varg_Execution)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void Call_NextAnimationKey(const _uint& In_iKeyIndex);

protected:
    virtual void OnDisable() override;
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

public:
    void Call_AnimationEnd(_uint iEndAnimIndex);

private:
    weak_ptr<CGameObject> m_pTargetObject;
    _uint m_iDustEffectIndex;

    weak_ptr<CAnimation> m_ThisStateAnimationCom;

protected:
    virtual void OnEventMessage(weak_ptr<CBase> pArg) override;
    void Free();
    virtual void OnDestroy() override;
    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;

};

END