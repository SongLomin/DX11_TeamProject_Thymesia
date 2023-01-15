#pragma once
#include "BossUrd/UrdBossStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUrdBossState_Start :
	public CUrdBossStateBase
{
	GAMECLASS_H(CUrdBossState_Start);
	CLONE_H(CUrdBossState_Start, CComponent)
		SHALLOW_COPY(CUrdBossState_Start)

public:
	_matrix Get_PlayerTransform() {
		return XMLoadFloat4x4(&m_vPlyerMatrix);
	}

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
	virtual void OnEventMessage(_uint iArg) override;

private:
	_bool      m_bNextState = false;
	_float     m_fSinematic = 0.f;
	_bool      m_bSinematicStart = false;
	_float4x4  m_vPlyerMatrix;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END






