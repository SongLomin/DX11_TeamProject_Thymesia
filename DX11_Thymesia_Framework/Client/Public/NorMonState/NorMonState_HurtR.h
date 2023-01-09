#pragma once
#include "NorMonsterStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_HurtR :
	public CNorMonsterStateBase
{
	GAMECLASS_H(CNorMonState_HurtR);
	CLONE_H(CNorMonState_HurtR, CComponent)
		SHALLOW_COPY(CNorMonState_HurtR)

public:
	void    Set_ParryCount(_uint ParryCount) { m_iParryCount += ParryCount; }
	void    Set_ZeroParryCount(_uint ParryCount) { m_iParryCount = ParryCount; }
	_uint   Get_ParryCount() { return  m_iParryCount; }

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
	_uint  m_iParryCount = 0;

private:
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END















