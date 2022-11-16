#pragma once
#include "CorvusStateBase.h"
BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_Hit :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_Hit);
    SHALLOW_COPY(CCorvusState_Hit)
        CLONE_H(CCorvusState_Hit, CComponent)

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

protected:
    virtual void OnDestroy() override;
    void Free();


    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;

};

END