#pragma once
#include  "CorvusStates/CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_PS_VargSwordStart :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_PS_VargSwordStart);
    CLONE_H(CCorvusState_PS_VargSwordStart, CComponent)
        SHALLOW_COPY(CCorvusState_PS_VargSwordStart)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

public:
    void Call_AnimationEnd();
    void Call_NextKeyFrame(const _uint& In_KeyIndex);

private:
    weak_ptr<CGameObject> m_pTargetObject;
    _uint m_iDustEffectIndex;
	weak_ptr<CAnimation> m_pThisAnimationCom;

protected:
    virtual void OnEventMessage(weak_ptr<CBase> pArg) override;
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;
    void Free();

    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;
};

END