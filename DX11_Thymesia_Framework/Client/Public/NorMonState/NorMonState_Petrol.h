#pragma once
#include "NorMonsterStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_Petrol :
	public CNorMonsterStateBase
{
	GAMECLASS_H(CNorMonState_Petrol);
	CLONE_H(CNorMonState_Petrol, CComponent)
		SHALLOW_COPY(CNorMonState_Petrol)

public:
	_float      Cul_DotResult();
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
	_float m_fMaxSpeed = 2.5f;
	_float m_fCurrentSpeed = 1.2f;
	_float m_fAccel = 0.f;


	//TODO юс╫ц
	vector<_float3>          m_vecPatrolPosition;
	_int            m_iPatrolCount = 0;
	_int            m_iCorssResult = 0;
	_bool           m_bTurnCheck = false;
	
	virtual void OnEventMessage(_uint iArg) override;

private:
//	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	//virtual void OnDestroy() override;
	void Free();

};

END

