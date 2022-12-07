#pragma once
#include  "CorvusStates/CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_RaidAttack1Hurt :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_RaidAttack1Hurt);
    CLONE_H(CCorvusState_RaidAttack1Hurt, CComponent)
        SHALLOW_COPY(CCorvusState_RaidAttack1Hurt)

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
    void Init_Varg(weak_ptr<CModel> In_pModelCom, weak_ptr<CTransform> In_ParentTransformCom, const string& szTargetNode = "WeaponCase1");

private:
    _uint m_iDustEffectIndex;
    _float4x4  m_TransformationMatrix;

protected:
    void Free();

    // CNorMonStateBase을(를) 통해 상속됨
    virtual _bool Check_AndChangeNextState() override;

};

END