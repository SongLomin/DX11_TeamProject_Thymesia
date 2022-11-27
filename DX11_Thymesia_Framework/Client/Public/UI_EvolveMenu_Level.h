#pragma once
#include "UI_Container.h"
#include "Status_Player.h"

BEGIN(Client)

CLIENT_DECLATION_UI

class CFadeMask;
class CEasingTransform;

class CUI_EvolveMenu_Level : public CUI_Container
{
public:
	enum class  EVOLVE_LEVEL_TYPE { STR, VIT, PLA, APPLY, EVOLVE_LEVEL_TYPE_END };
	enum class	LEVEL_RECONFIRM_TYPE{YES,NO,LEVEL_RECONFIRM_END};


public:
	GAMECLASS_H(CUI_EvolveMenu_Level)
		CLONE_H(CUI_EvolveMenu_Level, CGameObject)


public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void   Tick(_float fTimeDelta);
	virtual void   LateTick(_float fTimeDelta);



private:
	void	Create_Background();
	void	Create_Memory();
	void	Create_Status();
	void	Create_State();
	void	Create_NoneGrouping();
	void	Create_ReconfirmWindow();

	void	Init_OriginFontInfo();
	void	Init_ChangeFontInfo();
	void	Update_FontInfo();
	void	Update_ChangeStatus();


private:
	weak_ptr<CCustomUI> m_pPauseMenuBackground;
	weak_ptr<CCustomUI> m_pPauseMenuBackground_Main;
	weak_ptr<CCustomUI> m_pPauseMenuBackground_Top;



private:
	weak_ptr<CCustomUI> m_pTitle;
	weak_ptr<CCustomUI>   m_pLevel;
	weak_ptr<CCustomUI> m_pMemory;
	weak_ptr<CCustomUI> m_pRequired_Memory;



	//Status
private:
	weak_ptr<CCustomUI>   m_pStatusBG;

	weak_ptr<CCustomUI>   m_pStatusTitle;
	weak_ptr<CCustomUI>   m_pStatusStrength;
	weak_ptr<CCustomUI>   m_pStatusVitality;
	weak_ptr<CCustomUI>   m_pStatusPlague;


	//State
private:
	weak_ptr<CCustomUI>   m_pState_Title;
	weak_ptr<CCustomUI>   m_pState_AttackDamage;
	weak_ptr<CCustomUI>   m_pState_Wound;
	weak_ptr<CCustomUI>   m_pState_ClawDamage;
	weak_ptr<CCustomUI>   m_pState_HP;
	weak_ptr<CCustomUI>   m_pState_MP;



private:

	weak_ptr<CCustomUI>   m_pApply;
	weak_ptr<CCustomUI>   m_pHighlight;
	weak_ptr<CCustomUI>   m_pStatusArrowLeft;
	weak_ptr<CCustomUI>   m_pStatusArrowRight;

	_uint            m_iArrowArraySize;
	_float2            m_fDecorationArrowPos[11];

private:
	EVOLVE_LEVEL_TYPE			m_eSelectType;
	_uint						m_iSelectedIndex;
	_float						m_fFontScale = 0.87f;
	_float						m_fFontOriginSize = 23.f;

	_float						m_fFontSize;
	_float						m_fFontOffset;
private://ApplyReconfirm
	_bool				m_bOpenableReconfirmWindow;
	_bool				m_bOpenReconfirmWindowThisFrame;
	_uint				m_iReconfirmWindowIndex;

	weak_ptr<CCustomUI>	m_pReconfirmWindowBG		;
	weak_ptr<CCustomUI>	m_pReconfirmWindowNotice	;
	weak_ptr<CCustomUI>	m_pReconfirmWindowYes		;
	weak_ptr<CCustomUI>	m_pReconfirmWindowNo		;
	weak_ptr<CCustomUI>	m_pReconfirmWindowHighlight	;



private:
	CStatus_Player::PLAYERDESC m_tOriginStatus;
	CStatus_Player::PLAYERDESC m_tChangeStatus;
	_uint						m_iNeedMemory = 0;

private:
	TEXTINFO					m_tTextInfo_OriginLevel;
	TEXTINFO					m_tTextInfo_OriginMemory;

	TEXTINFO					m_tTextInfo_OriginStr;
	TEXTINFO					m_tTextInfo_OriginVit;
	TEXTINFO					m_tTextInfo_OriginPlague;



	TEXTINFO					m_tTextInfo_OriginAttackDamage;
	TEXTINFO					m_tTextInfo_OriginWound;
	TEXTINFO					m_tTextInfo_OriginClawDamage;
	TEXTINFO					m_tTextInfo_OriginHP;
	TEXTINFO					m_tTextInfo_OriginMP;

private:
	TEXTINFO					m_tTextInfo_ChangeLevel;
	TEXTINFO					m_tTextInfo_ChangeMemory;
	TEXTINFO					m_tTextInfo_RequireMemory;

	TEXTINFO					m_tTextInfo_ChangeStr;
	TEXTINFO					m_tTextInfo_ChangeVit;
	TEXTINFO					m_tTextInfo_ChangePlague;


	TEXTINFO					m_tTextInfo_ChangeAttackDamage;
	TEXTINFO					m_tTextInfo_ChangeWound;
	TEXTINFO					m_tTextInfo_ChangeClawDamage;
	TEXTINFO					m_tTextInfo_ChangeHP;
	TEXTINFO					m_tTextInfo_ChangeMP;


	vector<TEXTINFO>			m_vecTextInfo;

protected:
	void			OnDisable() override;

	


private:
	TEXTINFO         m_tTextInfo_Memory;

private:
	weak_ptr<CEasingTransform> m_pEasingTransformCom;


private:
	void            ChangeSelectedIndex();
	void            SelectButton();
	void            OpenReconfirmWindow();
	void            TickReconfirmWindow();
	



private:
	virtual void   OnEnable(void* pArg) override;


private:
	weak_ptr<CFadeMask>   m_pFadeMask;


public:
	void            Call_ReturnToEvolveMenu();
};

END