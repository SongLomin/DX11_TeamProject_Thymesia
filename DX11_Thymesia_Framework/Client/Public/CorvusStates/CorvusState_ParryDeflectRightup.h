#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_ParryDeflectRightup :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_ParryDeflectRightup);
    CLONE_H(CCorvusState_ParryDeflectRightup, CComponent)
        SHALLOW_COPY(CCorvusState_ParryDeflectRightup)

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

    virtual void HitEffectSound() {}

public:
    void Call_AnimationEnd(_uint iEndAnimIndex);
    void Play_AttackWithIndex(const _tchar& In_iAttackIndex);

private:
    _uint m_iDustEffectIndex;
    
    _bool m_bParry1ORParry2 = false;


protected:
    void Free();
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;
    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;



};

END
