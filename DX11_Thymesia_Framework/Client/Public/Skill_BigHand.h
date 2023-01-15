#pragma once
#include "Skill_Base.h"

BEGIN(Client)

class CSkill_BigHand: public CSkill_Base
{
public:
	GAMECLASS_H(CSkill_BigHand)
		CLONE_H(CSkill_BigHand, CComponent)
		SHALLOW_COPY(CSkill_BigHand)

public:
	virtual void Start() override;
private:
	virtual void Init_SkillInfo() override;
	virtual void Init_State() override;


private:
	void		Free();
};
END