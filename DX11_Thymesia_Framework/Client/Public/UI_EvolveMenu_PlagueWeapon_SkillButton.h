#pragma once
#include "UI_Button.h"

BEGIN(Client)

class CEasingComponent_Alpha;
class CCustomUI;
class CUI_EvolveMenu_SKillIcon;
class CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot;


class CUI_EvolveMenu_PlagueWeapon_SkillButton : public CUI_Button
{
public:
	GAMECLASS_H(CUI_EvolveMenu_PlagueWeapon_SkillButton)
	CLONE_H(CUI_EvolveMenu_PlagueWeapon_SkillButton, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

private:
	_bool			Check_UnLocked();

public:
	virtual void    Set_UIPosition(const _float fX, const _float fY) override;
	void			Set_Skill(SKILL_NAME eSkillName);

	SKILL_NAME		Get_SkillName() { return m_eSkillName; }
	_bool			Get_Unlocked();
	_bool			Get_Unlockable();

	_uint			Get_RequirementSkillPiece();

	void			Unlock_Skill();
	void			Set_Slot(weak_ptr<CUI> pSlot);
	weak_ptr<CUI>	Get_CurrentSlot() { return m_pMyCurrentSlot; };
	weak_ptr<CUI>	Get_OriginSlot() { return m_pMyOriginSlot; }


	void			SetPosToMyCurrentSlot();
	void			SetPosToMyOriginSlot();



private:
	void			SetUp_UI();

protected:
	virtual void	OnMouseOver() override;

	virtual void	OnLButtonDown() override; 
	virtual void	OnMouseOut() override;
	virtual void	OnLButtonUp() override; 

private:
	weak_ptr<CCustomUI>			m_pHover;
	weak_ptr<CCustomUI>			m_pFrame;
	weak_ptr<CUI_EvolveMenu_SKillIcon>			m_pIcon;

	weak_ptr<CUI>	 m_pMyOriginSlot;
	weak_ptr<CUI>	 m_pMyCurrentSlot;

public:
	FDelegate<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>	Callback_MouseOver;
	FDelegate<>														Callback_MouseOut;
	FDelegate<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>	Callback_UnLockSkill;

	FDelegate<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>	Callback_ButtonUp;
	FDelegate<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>	Callback_ButtonDown;


private:
	virtual void				OnEnable(void* pArg) override;
	virtual void				OnDisable() override ;

private:
	SKILLBUTTON_LOCK_TYPE m_eLockType = SKILLBUTTON_LOCK_TYPE::LOCKED;
	SKILL_NAME			m_eSkillName = SKILL_NAME::SKILL_END;


};

END

