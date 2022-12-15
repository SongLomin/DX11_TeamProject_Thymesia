#pragma once
#include "RequirementBase.h"

BEGIN(Client)

class CStatus_Player;

class CRequirement_PlayerStatusMana : public CRequirementBase
{
public:
	virtual HRESULT     Initialize(void* pArg) override;
public:
	virtual _bool Check_Requirement() override;
	virtual _bool Is_Valid() override;
	void	Init_Req(_float fRequireMana);

private:
	_float			m_fRequireMana = 0.f;
	_bool			m_bIsValid = true;
	weak_ptr<CStatus_Player>	m_pStatus_Player;
};

END
