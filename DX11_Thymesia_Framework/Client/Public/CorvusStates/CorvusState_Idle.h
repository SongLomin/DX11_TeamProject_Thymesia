#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
class CPhysXController;
END

BEGIN(Client)

class CCorvusState_Idle :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_Idle);
    CLONE_H(CCorvusState_Idle, CComponent)
        SHALLOW_COPY(CCorvusState_Idle)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;
    virtual _bool Check_AndChangeNextState() override;

private:
    _bool m_bLadderLock = false;
    //TODO �ӽ�
    _uint m_iSkillType = 0;

protected:
    void Free();

};

END