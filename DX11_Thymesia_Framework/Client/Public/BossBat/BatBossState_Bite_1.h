#pragma once
#include "BossBat/BatBossStateBase.h"

BEGIN(Engine)
class CModel;
class CAnimation;
END

BEGIN(Client)

class CBatBossState_Bite_1 :
	public CBatBossStateBase
{
	GAMECLASS_H(CBatBossState_Bite_1);
	CLONE_H(CBatBossState_Bite_1, CComponent)
		SHALLOW_COPY(CBatBossState_Bite_1)



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
	void Call_NextAnimationKey(const _uint& In_iKeyIndex);


private:
	weak_ptr<CAnimation> m_ThisStateAnimationCom;

private:
	_bool    m_bNextState = false;
	_float   m_fSinematic = 0.f;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






