#pragma once
#include  "CorvusStates/CorvusState_PS.h"

BEGIN(Client)
class CCorvusState_PS_CaneSword_Start final :
    public CCorvusState_PS
{
    GAMECLASS_H(CCorvusState_PS_CaneSword_Start);
    CLONE_H(CCorvusState_PS_CaneSword_Start, CComponent);
    SHALLOW_COPY(CCorvusState_PS_CaneSword_Start);

public:
	void Call_AnimationEnd(_uint iEndAnimIndex);
	virtual void Call_NextKeyFrame(const _uint& In_KeyIndex) override;

private:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

	virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
	virtual void OnStateEnd() override;
	virtual void OnEventMessage(weak_ptr<CBase> pArg) override;
	void Free();
};

END
