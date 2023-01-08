#pragma once
#include "Skill_Base.h"

BEGIN(Client)

class CSkill_BloodStorm : public CSkill_Base
{
public:
	GAMECLASS_H(CSkill_BloodStorm)
		CLONE_H(CSkill_BloodStorm, CComponent)
		SHALLOW_COPY(CSkill_BloodStorm)

public:
	virtual void Start() override;
private:
	virtual void Init_SkillInfo() override;
	virtual void Init_State() override;


private:
	void		Free();
};
END