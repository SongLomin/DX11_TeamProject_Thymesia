#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_Getup :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_Getup);
    CLONE_H(CCorvusState_Getup, CComponent)
        SHALLOW_COPY(CCorvusState_Getup)

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
    void Call_AnimationEnd(_uint iEndAnimIndex);

private:
    _uint m_iDustEffectIndex;

protected:
    void Free();
    virtual void OnDestroy() override;
    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;

};

END