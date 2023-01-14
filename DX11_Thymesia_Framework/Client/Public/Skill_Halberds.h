#pragma once
#include "Skill_Base.h"

BEGIN(Client)

class CSkill_Halberds : public CSkill_Base
{
public:
	GAMECLASS_H(CSkill_Halberds)
	CLONE_H(CSkill_Halberds, CComponent)
	SHALLOW_COPY(CSkill_Halberds)

public:
	virtual void Start() override;
private:
	virtual void Init_SkillInfo() override;
	virtual void Init_State() override;


private:
	void		Free();
};
END