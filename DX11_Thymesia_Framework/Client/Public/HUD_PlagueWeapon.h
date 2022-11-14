#pragma once
#include "UI.h"

BEGIN(Client)

class CCustomUI;
class CPlayer_ProgressBar;
class CHUD_Hover;


class CHUD_PlagueWeapon :
    public CUI
{
	GAMECLASS_H(CHUD_PlagueWeapon);
	CLONE_H(CHUD_PlagueWeapon, CGameObject);
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
public:
	/*Delegate Funtion*/
	void		Call_ChangeSkill();
	void		Call_UseSkill();
	

public:
	//HoverEvent
	void			Call_HoverFadeEnd(FADER_TYPE eFaderType);

protected:
	
private:
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Main;//프레임
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Icon;//아이콘
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Ready;//쿨타임시에만 나오는 회색 막
	weak_ptr<CPlayer_ProgressBar>	m_pPlagueWeapon_Border;//쿨타임 알려주는 게이지 프레임
	weak_ptr<CHUD_Hover> m_pHover;//스킬 쿨 다 돌았을시 나오는 이펙트.

private:
	Engine::FaderDesc m_tFaderDesc; //For HUD_Hover;


private:
	void			Hover_Event();



private:
	_float			m_fCoolTime;
	_float			m_fNowTime;
	_bool			m_bIsCoolTime;
public:
	void Free();

};

END