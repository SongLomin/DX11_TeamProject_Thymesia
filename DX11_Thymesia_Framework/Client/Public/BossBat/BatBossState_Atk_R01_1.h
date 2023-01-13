#pragma once
#include "BossBat/BatBossStateBase.h"

BEGIN(Engine)
class CModel;
class CAnimation;
END

BEGIN(Client)

class CBatBossState_Atk_R01_1:
	public CBatBossStateBase
{
	GAMECLASS_H(CBatBossState_Atk_R01_1);
	CLONE_H(CBatBossState_Atk_R01_1, CComponent)
		SHALLOW_COPY(CBatBossState_Atk_R01_1)



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

protected:

	void Free();

};

END






