#pragma once
#include  "CorvusStates/CorvusState_PS.h"

BEGIN(Client)
class CCorvusState_PS_Scythe final :
    public CCorvusState_PS
{
    GAMECLASS_H(CCorvusState_PS_Scythe);
    CLONE_H(CCorvusState_PS_Scythe, CComponent);
    SHALLOW_COPY(CCorvusState_PS_Scythe);

public:
	void Call_AnimationEnd(_uint iEndAnimIndex);
	virtual void Call_NextKeyFrame(const _uint& In_KeyIndex) override;

private:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual _bool Check_AndChangeNextState() override;

	virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
	virtual void OnStateEnd() override;
	virtual void OnEventMessage(weak_ptr<CBase> pArg) override;
	void Free();
};

END
