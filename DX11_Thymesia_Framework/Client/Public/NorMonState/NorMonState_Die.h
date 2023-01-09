#pragma once
#include "NorMonsterStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_Die :
	public CNorMonsterStateBase
{
	GAMECLASS_H(CNorMonState_Die);
	CLONE_H(CNorMonState_Die, CComponent)
	SHALLOW_COPY(CNorMonState_Die)

public:
	void Call_NextKeyFrame(const _uint& In_KeyIndex);

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

	virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
	virtual void OnStateEnd() override;
	virtual _bool Check_AndChangeNextState() override;
	virtual void OnDestroy() override;

private:
	_float      m_fDissolveTime;
	_bool       m_bAnimEnd;

	
	
private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
	void Free();


};

END

