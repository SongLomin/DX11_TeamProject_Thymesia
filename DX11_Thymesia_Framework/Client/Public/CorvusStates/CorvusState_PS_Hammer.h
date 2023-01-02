#pragma once
#include  "CorvusStates/CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_PS_Hammer :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_PS_Hammer);
    CLONE_H(CCorvusState_PS_Hammer, CComponent)
        SHALLOW_COPY(CCorvusState_PS_Hammer)

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

public:
    void Call_AnimationEnd();

private:
    weak_ptr<CGameObject> m_pTargetObject;
    _uint m_iDustEffectIndex;

protected:
    virtual void OnEventMessage(weak_ptr<CBase> pArg) override;
    void Free();

    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;

};

END