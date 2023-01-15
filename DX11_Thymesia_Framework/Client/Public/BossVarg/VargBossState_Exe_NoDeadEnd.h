#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_Exe_NoDeadEnd :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_Exe_NoDeadEnd);
	CLONE_H(CVargBossState_Exe_NoDeadEnd, CComponent)
		SHALLOW_COPY(CVargBossState_Exe_NoDeadEnd)



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

public:
	void Set_DeadChoice(_bool DeadChoice) { m_bDeadChoice = DeadChoice; }


private:
	_bool  m_bDeadChoice = false;
private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
	virtual void OnEventMessage(_uint iArg) override;
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






