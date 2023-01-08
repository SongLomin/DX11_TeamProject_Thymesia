#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CJavelinWeapon;

class CUrdBossState_Skill02_1 :
	public CUrdBossStateBase
{
	GAMECLASS_H(CUrdBossState_Skill02_1);
	CLONE_H(CUrdBossState_Skill02_1, CComponent)
		SHALLOW_COPY(CUrdBossState_Skill02_1)



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
	_bool m_bOne = false;
	_bool m_bDisableWeaponCheck = false;
	weak_ptr<CJavelinWeapon> pJavelinWeapon;
private:
	void Call_AnimationEnd();
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






