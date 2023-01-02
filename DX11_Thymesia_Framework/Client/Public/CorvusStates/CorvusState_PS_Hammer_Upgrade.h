#pragma once
#include  "CorvusStates/CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_PS_Hammer_Upgrade :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_PS_Hammer_Upgrade);
    CLONE_H(CCorvusState_PS_Hammer_Upgrade, CComponent)
        SHALLOW_COPY(CCorvusState_PS_Hammer_Upgrade)

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
    void Call_NextKeyFrame(const _uint& In_KeyIndex);

private:
    weak_ptr<CGameObject> m_pTargetObject;
    _uint m_iDustEffectIndex;

    weak_ptr<CAnimation> m_pThisAnimationCom;

protected:
    virtual void OnEventMessage(weak_ptr<CBase> pArg) override;
    void Free();

    // CNorMonStateBase��(��) ���� ��ӵ�
    virtual _bool Check_AndChangeNextState() override;

};

END