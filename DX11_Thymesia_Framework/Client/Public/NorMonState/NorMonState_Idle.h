#pragma once
#include "AIStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_Idle :
	public CAIStateBase
{
	GAMECLASS_H(CNorMonState_Idle);
	CLONE_H(CNorMonState_Idle, CComponent)
		SHALLOW_COPY(CNorMonState_Idle)


public:
	void Set_MonIdleType(NORMONSTERIDLETYPE IDLETYPE);
	void Set_IdleType(_uint IdleType) { m_iIdleType = IdleType;}
	void Set_ClosePlayerCheck(_bool ClosePlayerCheck) 	{m_bClosePlayerCheck = ClosePlayerCheck;}
	void Set_CloseToRun(_bool CloseToRun) { m_bCloseToRun = CloseToRun; }
	void Set_AnimIndex(_uint AnimIndex) { m_iAnimIndex = AnimIndex; }


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


private: // 아이들에서만쓰는불값
	_bool     m_bClosePlayerCheck = false;
	_bool     m_bCloseToRun = false;
	_uint     m_iIdleType = 0; // 이거 enum으로 관리하면될듯  다하고할거임 ㅇㅇ






protected:
	void Free();

};

END