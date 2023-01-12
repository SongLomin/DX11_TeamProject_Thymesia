#pragma once
#include "UI_Button.h"
#include "CustomUI.h"


BEGIN(Client)

class CEasingComponent_Alpha;
class CUI_EvolveMenu_PlagueWeapon_SkillButton;
class CUI_EvolveTalent_Active;

class CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot : public CUI_Button
{
public:
	GAMECLASS_H(CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot)
	CLONE_H(CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	void			Equip_Skill(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkill, _uint iSkillSocketType);
	void			Clear_Slot();
	void			UnEquip_Skill(_uint iSkillSocketType);
	_bool			IsEquipedSkill();
	weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>	Get_Equiped_Skill() { return m_pEquipSkill; }

private:
	void			Init_UI();

private:
	void			OnEnable(void* pArg);

private:
	weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillButton>	m_pEquipSkill;
	weak_ptr<CCustomUI>						m_pFrame;
	weak_ptr<CUI_EvolveTalent_Active>		m_pActive;
};

END

