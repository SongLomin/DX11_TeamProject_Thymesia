 #pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_Die :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_Die);
    CLONE_H(CCorvusState_Die, CComponent)
        SHALLOW_COPY(CCorvusState_Die)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

protected:
    virtual void OnDisable() override;

    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;

public:
    //void Call_AnimationEnd(_uint iEndAnimIndex);

private:
    _uint m_iDustEffectIndex;
    _bool m_bAnimPlay = true;

    _float      m_fDissolveTime;
    _bool       m_bDissolve = true;

    _float m_fGrayScaleValue = 1.f;

    _bool m_bFadeOutTrigger = true;
protected:
    void Free();

    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;

};

END
