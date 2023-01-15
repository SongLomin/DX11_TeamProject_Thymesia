#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Client)
class CUrdBossState_Attacks abstract :
	public CUrdBossStateBase
{
	GAMECLASS_H(CUrdBossState_Attacks);

public:
	virtual void Call_AnimationEnd(_uint iEndAnimIndex) = 0;
	virtual void Call_NextKeyFrame(const _uint& In_KeyIndex) = 0;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

	virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
	virtual void OnStateEnd() override;

	virtual _bool Check_AndChangeNextState() = 0;

	void Set_MoveScale(const _float fRatio = 1.f);

	_bool Is_Urd_Phase1();

	void TurnOn_Effect(const std::string& szEffectKey);
	void TurnOff_Effect(const std::string& szEffectKey);

	virtual void OnDestroy() override;
	void Free();

protected:
	_float m_fTurnAtkSpeedRatio = 1.f;
	weak_ptr<CAnimation> m_pThisAnimationCom;
};

END






