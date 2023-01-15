#pragma once
#include "NorMonsterStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_TakeExecution :
	public CNorMonsterStateBase
{
	GAMECLASS_H(CNorMonState_TakeExecution);
	CLONE_H(CNorMonState_TakeExecution, CComponent)
		SHALLOW_COPY(CNorMonState_TakeExecution)

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
	_float      m_fDissolveTime;
	_bool       m_bAnimEnd;
	_bool       m_bTargetLook = false;
	_float      m_fOffSetX;
	_float      m_fOffSetZ;


private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END



