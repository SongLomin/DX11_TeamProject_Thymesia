#pragma once
#include  "CorvusStates/CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_Execution_R_R :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_Execution_R_R);
    CLONE_H(CCorvusState_Execution_R_R, CComponent)
        SHALLOW_COPY(CCorvusState_Execution_R_R)

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

    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

public:
    void Call_AnimationEnd(_uint iEndAnimIndex);

private:
    weak_ptr<CGameObject> m_pTargetObject;
    _uint m_iDustEffectIndex;
    weak_ptr<CAnimation> m_ThisStateAnimationCom;
    MONSTERTYPE m_eExcutionTargetType = MONSTERTYPE::TYPE_END;
    _bool m_bExcution = false;
protected:
    virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;
    void Free();
    virtual void OnDestroy() override;
    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;

private:
    virtual void OnEventMessage(_uint iArg) override;

};

END