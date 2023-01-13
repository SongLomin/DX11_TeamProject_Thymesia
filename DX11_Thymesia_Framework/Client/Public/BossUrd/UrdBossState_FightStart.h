#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Engine)
class CModel;
class CAnimation;
END

BEGIN(Client)

class CUrdBossState_FightStart :
	public CUrdBossStateBase
{
	GAMECLASS_H(CUrdBossState_FightStart);
	CLONE_H(CUrdBossState_FightStart, CComponent)
		SHALLOW_COPY(CUrdBossState_FightStart)


public:
	void Call_NextKeyFrame(const _uint& In_KeyIndex);

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
	DECAL_DESC m_DecalDesc;
	weak_ptr<CAnimation> m_pThisAnimationCom;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






