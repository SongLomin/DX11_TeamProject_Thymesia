#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
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

protected:
    virtual void Free() override;

};

END