#pragma once
#include "EliteMonState/BigHandMan/BigHandManStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBigHandManState_ComboB_Start :
	public CBigHandManStateBase
{
	GAMECLASS_H(CBigHandManState_ComboB_Start);
	CLONE_H(CBigHandManState_ComboB_Start, CComponent)
		SHALLOW_COPY(CBigHandManState_ComboB_Start)

public:
	void Call_NextKeyFrame(const _uint& In_KeyIndex);

public:
	void   Set_FourAttackEnd( _bool bFourAttackRun)   { m_bFourAttackEnd = bFourAttackRun; }
	void   Set_OneAttackEnd( _bool  bOneAttackEnd)  { m_bOneAttackEnd = bOneAttackEnd; }
	void   Set_OneAttackRun( _bool bOneAttackRun)   { m_bOneAttackRun = bOneAttackRun; }

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
	_bool          m_bFourAttackEnd = false;
	_bool          m_bOneAttackEnd = false;
	_bool          m_bOneAttackRun = false;


	_uint          m_iAttackCount = 0;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);


protected:
	virtual void OnDestroy() override;
	void Free();

};

END






