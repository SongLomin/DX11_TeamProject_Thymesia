#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_Climb_R_Down :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_Climb_R_Down);
    SHALLOW_COPY(CCorvusState_Climb_R_Down)
    CLONE_H(CCorvusState_Climb_R_Down, CComponent)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void Call_AnimationEnd(_uint iEndAnimIndex);
    void Call_NextKeyFrame(const _uint& In_KeyIndex);


private:
    weak_ptr<CAnimation> m_pThisAnimationCom;
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