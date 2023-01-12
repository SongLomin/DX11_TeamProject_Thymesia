#pragma once
#include "NorMonsterStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_GroggyLoop :
	public CNorMonsterStateBase
{
	GAMECLASS_H(CNorMonState_GroggyLoop);
	CLONE_H(CNorMonState_GroggyLoop, CComponent)
		SHALLOW_COPY(CNorMonState_GroggyLoop)

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
	virtual void OnEventMessage(_uint iArg) override;

private:
	_float     m_fLoopEndTime = 0.f;
	_bool   m_bNextStatepause = false;
	_uint   m_iCurrentKeyIndex = 0;

protected:

	void Free();

};

END



