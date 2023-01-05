#pragma once
#include "EliteMonState/BigHandMan/BigHandManStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBigHandManState_ComboB_Loop :
	public CBigHandManStateBase
{
	GAMECLASS_H(CBigHandManState_ComboB_Loop);
	CLONE_H(CBigHandManState_ComboB_Loop, CComponent)
		SHALLOW_COPY(CBigHandManState_ComboB_Loop)

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
	void Call_AnimationEnd();
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






