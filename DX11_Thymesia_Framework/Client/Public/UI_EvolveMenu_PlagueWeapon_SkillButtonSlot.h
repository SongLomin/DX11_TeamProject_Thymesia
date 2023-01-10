#pragma once
#include "UI_Button.h"


BEGIN(Client)

class CEasingComponent_Alpha;
class CCustomUI;

class CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot :
    public CUI_Button
{
public:
    GAMECLASS_H(CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot)
    CLONE_H(CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	virtual void	OnMouseOver() override;
	virtual void	OnMouseOut() override;

private:
	void			Init_UI();

private:
	void			OnEnable(void* pArg);
	void			OnDisable();


private:
	weak_ptr<CCustomUI>		m_pFrame;
	weak_ptr<CCustomUI>		m_pHoverEffect;

private:
	weak_ptr<CEasingComponent_Alpha>	 m_pHoverEffectEasingAlpha;

};

END

