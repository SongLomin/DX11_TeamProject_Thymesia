#pragma once
#include "UI_Container.h"

BEGIN(Client)

class CCustomUI;
class CFadeMask;
class CUI_EvolveMenu_TalentButton;
class CTalent;

class CUI_EveolveMenu_Talent : public CUI_Container
{
public:
	enum class TALENT_TAP{TALENT_SWORD, TALENT_PARRY, TALENT_TAP_END};

public:
	GAMECLASS_H(CUI_EveolveMenu_Talent)
	CLONE_H(CUI_EveolveMenu_Talent, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;


public:
	void	SetRootTalent(weak_ptr<CTalent> In_pTalent, TALENT_TAP eRootType);

private:
	void	Create_Background();
	void	Create_TalentInformation();

	void	Init_Tap();

	void	Update_UI();

private:
	virtual void	OnEnable(void* pArg) override;
	virtual void	OnDisable() override;

	virtual void	UI_ChangeTap();

private:
	weak_ptr<CCustomUI> m_pPauseMenuBackground;
	weak_ptr<CCustomUI> m_pPauseMenuBackground_Main;
	weak_ptr<CCustomUI> m_pPauseMenuBackground_Top;
	weak_ptr<CCustomUI>	m_pTitle;


private:
	weak_ptr<CCustomUI> m_pTap[(_uint)TALENT_TAP::TALENT_TAP_END];
	weak_ptr<CCustomUI>	m_pTapTitle[(_uint)TALENT_TAP::TALENT_TAP_END];
	
	weak_ptr<CTalent>	m_pRoot[(_uint)TALENT_TAP::TALENT_TAP_END];


private:


private:
	weak_ptr<CCustomUI>	m_pTalentImage;
	weak_ptr<CCustomUI>	m_pTalentTitle;
	weak_ptr<CCustomUI>	m_pTalentInformation;

private:
	weak_ptr<CCustomUI> m_pMediaFrame;
	weak_ptr<CCustomUI>	m_pTalentInformationBG;


private:
	void	SetUp_TalentNode(weak_ptr<CTalent> pNode, UI_DESC tUIDesc);

private:
	_uint				m_iTalentIndex = 0;

private:
	weak_ptr<CFadeMask>   m_pFadeMask;

private:
	_float				m_fTreeOffsetY;
	_float				m_fTreeOffsetX;

public:
	void            Call_ReturnToEvolveMenu();


};


END