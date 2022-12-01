#pragma once
#include "UI_Button.h"


BEGIN(Client)

class CCustomUI;

class CEvolveMenu_TalentButton : public CUI_Button_Base
{
public:
	GAMECLASS_H(CEvolveMenu_TalentButton)
	CLONE_H(CEvolveMenu_TalentButton, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	
private:
	void			Update_Child();

private:
	virtual void	OnMouseOver()		override;
	virtual void	OnMouseOut()		override;
	virtual void	OnLButtonDown()		override;
	virtual void	OnLButtonClick()	override;

private:
	virtual void	OnEnable(void* pArg) override;

public:
	_bool			m_bActive;


public:
	weak_ptr<CCustomUI>	m_pButtonFrame;//프레임
	weak_ptr<CCustomUI>	m_pButtonActive;//버튼 눌렸을대
	weak_ptr<CCustomUI> m_pIcon;
};

END