#pragma once
#include "NorMonsterStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_Walk_F :
	public CNorMonsterStateBase
{
	GAMECLASS_H(CNorMonState_Walk_F);
	CLONE_H(CNorMonState_Walk_F, CComponent)
		SHALLOW_COPY(CNorMonState_Walk_F)

public:
	void  Set_GardnerAtkIndex(_uint GardnerAtkIndex) { m_iGardnerAtkIndex = GardnerAtkIndex; }
	void  Set_GardnerPlusAtkIndex(_uint GardnerAtkIndex) { m_iGardnerAtkIndex += GardnerAtkIndex; }
	void  Set_WalkCheck(_bool WalkCheck) { m_bWalkCheck = WalkCheck; }

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
	_float m_fMaxSpeed = 5.f;
	_float m_fCurrentSpeed = 2.4f;
	_float m_fAccel = 0.f;
	_bool  m_bWalkCheck = false;
	_uint  m_iGardnerAtkIndex = 0;

	virtual void OnEventMessage(_uint iArg) override;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END

