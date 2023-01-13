#pragma once
#include "Skill_base.h"

BEGIN(Client)

class CStolenSkill : public CSkill_Base
{
public:
	GAMECLASS_H(CStolenSkill)
	CLONE_H(CStolenSkill, CComponent)
	SHALLOW_COPY(CStolenSkill)
public:
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual void    UseSkill() override;
	void			OnStealSkill(weak_ptr<CSkill_Base>	pSkill);
	virtual void	Start_Skill(_bool bExpansion = false) override;

private:
	void		Free();

};


END

