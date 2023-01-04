#pragma once
#include "UI_Button.h"

BEGIN(Client)

class CEasingComponent_Alpha;
class CCustomUI;


class CUI_EvolveMenu_PlagueWeapon_SkillButton : public CUI_Button_Base
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
	void			Check_Locked();

public:
	virtual void    Set_UIPosition(const _float fX, const _float fY) override;
	void			Set_Skill(SKILL_NAME eSkillName);

	SKILL_NAME		Get_SkillName() { return m_eSkillName; }
	_bool			Get_Unlocked();
	_uint			Get_RequirementSkillPiece();
private:
	void			SetUp_UI();

protected:
	virtual void	OnMouseOver() override;
	virtual void	OnMouseOut() override;
	virtual void	OnLButtonDown() override;

private:
	weak_ptr<CCustomUI>			m_pHover;
	weak_ptr<CCustomUI>			m_pFrame;
	weak_ptr<CCustomUI>			m_pBorder;
	weak_ptr<CCustomUI>			m_pIcon;

private:
	weak_ptr<CEasingComponent_Alpha> m_pBorderEasingAlpha;

public:
	FDelegate<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>	Callback_MouseOver;
	FDelegate<>														Callback_MouseOut;
	FDelegate<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>	Callback_UnLockSkill;

private:
	virtual void				OnEnable(void* pArg) override;
	virtual void				OnDisable() override ;

private:
	SKILLBUTTON_LOCK_TYPE m_eLockType = SKILLBUTTON_LOCK_TYPE::LOCKED;
	SKILL_NAME			m_eSkillName = SKILL_NAME::SKILL_END;


};

END

