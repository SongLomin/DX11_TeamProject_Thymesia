#pragma once
#include "AIStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_DorMant :
	public CAIStateBase
{
	GAMECLASS_H(CNorMonState_DorMant);
	CLONE_H(CNorMonState_DorMant, CComponent)
		SHALLOW_COPY(CNorMonState_DorMant)

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
	void Free();

};

END
