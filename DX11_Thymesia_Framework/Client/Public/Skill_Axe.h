#pragma once
#include "Skill_Base.h"

BEGIN(Client)

class CSkill_Axe : public CSkill_Base
{
public:
	GAMECLASS_H(CSkill_Axe)
	CLONE_H(CSkill_Axe, CComponent)
	SHALLOW_COPY(CSkill_Axe)

public:
	virtual void Start() override;
private:
	virtual void Init_SkillInfo() override;
	virtual void Init_State() override;


private:
	void		Free();
};
END