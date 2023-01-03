#pragma once
#include "Skill_base.h"

BEGIN(Client)

class CSkill_Scythe :
    public CSkill_Base
{
public:
    GAMECLASS_H(CSkill_Scythe)
    CLONE_H(CSkill_Scythe, CComponent)
    SHALLOW_COPY(CSkill_Scythe)
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
