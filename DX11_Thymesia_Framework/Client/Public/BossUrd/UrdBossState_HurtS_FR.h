#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUrdBossState_HurtS_FR :
	public CUrdBossStateBase
{
	GAMECLASS_H(CUrdBossState_HurtS_FR);
	CLONE_H(CUrdBossState_HurtS_FR, CComponent)
		SHALLOW_COPY(CUrdBossState_HurtS_FR)

public:
	void        Set_ParryCount(_int iParryCount) { m_iParryCount += iParryCount;}        
	void        Set_ParryZeroCount(_int iParryCount) { m_iParryCount = iParryCount; }
	const _int  Get_ParryCount() { return m_iParryCount; }
protected:

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
	virtual void OnEventMessage(_uint iArg) override;


private:
	_int   m_iParryCount = 0;
                  

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






