#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUrdBossState_HurtS_FL :
	public CUrdBossStateBase
{
	GAMECLASS_H(CUrdBossState_HurtS_FL);
	CLONE_H(CUrdBossState_HurtS_FL, CComponent)
		SHALLOW_COPY(CUrdBossState_HurtS_FL)

public:
	void        Set_ParryCount(_int iParryCount) { m_iParryCount += iParryCount; }
	void        Set_ParryStart(_bool bParryStart) { m_bParryStart = bParryStart; }
	void        Set_ParryZeroCount(_int iParryCount) { m_iParryCount = iParryCount; }
	const _int  Get_ParryCount() {return m_iParryCount;}
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
	_int   m_iParryCount = 0;
	_bool  m_bParryStart = false;

private:
	void Call_AnimationEnd();
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






