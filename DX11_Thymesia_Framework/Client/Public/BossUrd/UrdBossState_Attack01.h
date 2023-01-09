#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUrdBossState_Attack01 :
	public CUrdBossStateBase
{
	GAMECLASS_H(CUrdBossState_Attack01);
	CLONE_H(CUrdBossState_Attack01, CComponent)
		SHALLOW_COPY(CUrdBossState_Attack01)

public:
	void    Set_ComboATKAnimation(_bool bComboATKAnimation) { m_bComboATKAnimation = bComboATKAnimation; }

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
	_bool    m_bComboATKAnimation = false;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






