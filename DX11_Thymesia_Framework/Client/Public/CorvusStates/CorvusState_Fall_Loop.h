#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
class CPhysXController;
END

BEGIN(Client)

class CCorvusState_Fall_Loop :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_Fall_Loop);
    SHALLOW_COPY(CCorvusState_Fall_Loop)
    CLONE_H(CCorvusState_Fall_Loop, CComponent)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void Call_AnimationEnd(_uint iEndAnimIndex);

protected:
    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

protected:
    virtual void OnDestroy() override;
    void Free();


    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;

};

END