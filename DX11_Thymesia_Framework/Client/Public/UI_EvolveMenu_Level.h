#pragma once
#include "UI_Container.h"

BEGIN(Client)

CLIENT_DECLATION_UI

class CFadeMask;

class CUI_EvolveMenu_Level : public CUI_Container
{
public:
	enum class  EVOLVE_LEVEL_TYPE{STR, VIT, PLA, APPLY, EVOLVE_LEVEL_TYPE_END };


public:
	GAMECLASS_H(CUI_EvolveMenu_Level)
	CLONE_H(CUI_EvolveMenu_Level, CGameObject)


public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);



private:
	void    Create_Background();
	void	Create_Memory();
	void	Create_Status();
	void	Create_State();
	void	Create_NoneGrouping();
private:
	weak_ptr<CCustomUI> m_pPauseMenuBackground;
	weak_ptr<CCustomUI> m_pPauseMenuBackground_Main;
	weak_ptr<CCustomUI> m_pPauseMenuBackground_Top;



private:
	weak_ptr<CCustomUI> m_pTitle;
	weak_ptr<CCustomUI>	m_pLevel;
	weak_ptr<CCustomUI> m_pMemory;
	weak_ptr<CCustomUI> m_pRequired_Memory;



	//Status
private:
	weak_ptr<CCustomUI>	m_pStatusBG;

	weak_ptr<CCustomUI>	m_pStatusTitle;
	weak_ptr<CCustomUI>	m_pStatusStrength;
	weak_ptr<CCustomUI>	m_pStatusVitality;
	weak_ptr<CCustomUI>	m_pStatusPlague;


	//State
private:
	weak_ptr<CCustomUI>	m_pState_Title;
	weak_ptr<CCustomUI>	m_pState_AttackDamage;
	weak_ptr<CCustomUI>	m_pState_Wound;
	weak_ptr<CCustomUI>	m_pState_ClawDamage;
	weak_ptr<CCustomUI>	m_pState_HP;
	weak_ptr<CCustomUI>	m_pState_MP;



private:

	weak_ptr<CCustomUI>	m_pApply;
	weak_ptr<CCustomUI>	m_pHighlight;
	weak_ptr<CCustomUI>	m_pStatusArrowLeft;
	weak_ptr<CCustomUI>	m_pStatusArrowRight;

	_uint				m_iArrowArraySize;
	_float2				m_fDecorationArrowPos[11];

private:
	EVOLVE_LEVEL_TYPE			m_eSelectType;
	_uint						m_iSelectedIndex;



private://ApplyReconfirm
	_bool				m_bOpenReconfirmWindow;



private:
	TEXTINFO			m_tTextInfo_Memory;



private:
	void				ChageSelectedIndex();
	void				SelectButton();






private:
	virtual void	OnEnable(void* pArg) override;


private:
	weak_ptr<CFadeMask>	m_pFadeMask;


public:
	void				Call_ReturnToEvolveMenu();
};

END
