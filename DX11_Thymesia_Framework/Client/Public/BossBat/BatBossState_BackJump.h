#pragma once
#include "BossBat/BatBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBatBossState_BackJump :
	public CBatBossStateBase
{
	GAMECLASS_H(CBatBossState_BackJump);
	CLONE_H(CBatBossState_BackJump, CComponent)
		SHALLOW_COPY(CBatBossState_BackJump)

public:
	void   Set_SpecialAtk(_bool SpecialAtk) { m_bSpecialAtk = SpecialAtk; }

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
private:
	_bool        m_bSpecialAtk = false;
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






