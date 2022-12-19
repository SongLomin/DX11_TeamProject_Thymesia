#pragma once
#include "JokerStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CJokerState_Sp_Open :
	public CJokerStateBase
{
	GAMECLASS_H(CJokerState_Sp_Open);
	CLONE_H(CJokerState_Sp_Open, CComponent)
	SHALLOW_COPY(CJokerState_Sp_Open)

public:
	void Call_NextKeyFrame(const _uint& In_KeyIndex);

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

	virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
	virtual void OnStateEnd() override;
	virtual _bool Check_AndChangeNextState() override;

	virtual void OnDestroy() override;
	void Free();

private:
	void Call_AnimationEnd();

private:
	_bool m_bCloseStart = false;
};

END






