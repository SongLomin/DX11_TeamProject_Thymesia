#pragma once
#include "Skill_base.h"

BEGIN(Client)

class CSkill_Hammer : public CSkill_Base
{
public:
    GAMECLASS_H(CSkill_Hammer)
    CLONE_H(CSkill_Hammer, CComponent)
    SHALLOW_COPY(CSkill_Hammer)

public:
	virtual void Start() override;
private:
	virtual void Init_SkillInfo() override;
	virtual void Init_State() override;

private:
	void		Free();

private:



};

END
