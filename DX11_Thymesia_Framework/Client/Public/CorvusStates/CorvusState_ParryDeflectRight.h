#pragma once
#include "CorvusStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCorvusState_ParryDeflectRight :
    public CCorvusStateBase
{
    GAMECLASS_H(CCorvusState_ParryDeflectRight);
    CLONE_H(CCorvusState_ParryDeflectRight, CComponent)
        SHALLOW_COPY(CCorvusState_ParryDeflectRight)

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;


private:
    void Check_InputAgainAvoid();

protected:
    virtual void OnDisable() override;

    virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
    virtual void OnStateEnd() override;

public:
    void Call_AnimationEnd();
    void Play_AttackWithIndex(const _tchar& In_iAttackIndex);

private:
    _uint m_iDustEffectIndex;
    
    _bool m_bParry1ORParry2 = false;

protected:
    void Free();

    // CNorMonStateBase��(��) ���� ��ӵ�
    virtual _bool Check_AndChangeNextState() override;



};

END
