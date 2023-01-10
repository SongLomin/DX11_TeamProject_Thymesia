#pragma once
#include "CorvusStateBase.h"
BEGIN(Engine)
class CModel;
class CTransform;
END

BEGIN(Client)

class CCorvusState_RunB :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_RunB);
    SHALLOW_COPY(CCorvusState_RunB)
    CLONE_H(CCorvusState_RunB, CComponent)

public:
    void Set_RunSpeed(const _float& In_fMaxSpeed, const _float& In_fAccel)
    {
        m_fMaxSpeed = In_fMaxSpeed;
        m_fAccel = In_fAccel;
    }

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    
protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;
private:
    _uint m_iDustEffectIndex;

private:
    _float m_fMaxSpeed = 8.f;
    _float m_fCurrentSpeed = 3.8f;
    _float m_fAccel = 0.f;

    _float m_fTimeAcc = 0.f;

protected:
    void Free();


    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;

};

END
