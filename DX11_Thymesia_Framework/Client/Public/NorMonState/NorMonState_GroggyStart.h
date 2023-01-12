#pragma once
#include "NorMonsterStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_GroggyStart :
	public CNorMonsterStateBase
{
	GAMECLASS_H(CNorMonState_GroggyStart);
	CLONE_H(CNorMonState_GroggyStart, CComponent)
	SHALLOW_COPY(CNorMonState_GroggyStart)

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
	_bool   m_bNextStatepause = false;
	_uint   m_iCurrentKeyIndex = 0;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
	virtual void OnEventMessage(_uint iArg) override;

protected:
	virtual void OnDestroy() override;
	void Free();

};

END


