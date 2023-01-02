#pragma once
#include "Skill_Base.h"

BEGIN(Client)


class CSkill_VargSword : public CSkill_Base
{
public:
	GAMECLASS_H(CSkill_VargSword)
	CLONE_H(CSkill_VargSword, CComponent)
	SHALLOW_COPY(CSkill_VargSword)

public:
	virtual void Start() override;
private:
	virtual void Init_SkillInfo() override;
	virtual void Init_State() override;
	

private:
	void		Free();
};
END