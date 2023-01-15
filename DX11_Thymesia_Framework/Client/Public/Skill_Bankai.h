#pragma once
#include "Skill_Base.h"

BEGIN(Client)

class CSkill_Bankai : public CSkill_Base
{
public:
	GAMECLASS_H(CSkill_Bankai)
	CLONE_H(CSkill_Bankai, CComponent)
	SHALLOW_COPY(CSkill_Bankai)

public:
	virtual void Start() override;
private:
	virtual void Init_SkillInfo() override;
	virtual void Init_State() override;


private:
	void		Free();
};
END