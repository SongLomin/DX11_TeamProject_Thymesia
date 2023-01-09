#pragma once
#include "VargBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CVargBossState_SPA_Run :
	public CVargBossStateBase
{
	GAMECLASS_H(CVargBossState_SPA_Run);
	CLONE_H(CVargBossState_SPA_Run, CComponent)
		SHALLOW_COPY(CVargBossState_SPA_Run)



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
	void   Set_Count(_bool iCount) { m_iCount = iCount; }

private:
	//TODO юс╫ц
	_uint m_iCount = 0;



private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






