#pragma once
#include "Skill_Base.h"

BEGIN(Client)

class CSkill_Knife : public CSkill_Base
{
public:
	GAMECLASS_H(CSkill_Knife)
	CLONE_H(CSkill_Knife, CComponent)
	SHALLOW_COPY(CSkill_Knife)

public:
	virtual void Start() override;
private:
	virtual void Init_SkillInfo() override;
	virtual void Init_State() override;


private:
	void		Free();
};
END