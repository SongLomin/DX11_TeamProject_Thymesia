#pragma once
#include "UI.h"

BEGIN(Client)

class CCustomUI;
class CPlayer_ProgressBar;
class CHUD_Hover;


class CHUD_PlagueWeapon_Steal :
	public CUI
{
	GAMECLASS_H(CHUD_PlagueWeapon_Steal);
	CLONE_H(CHUD_PlagueWeapon_Steal, CGameObject);
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


protected:

private:
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Main;//프레임
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Icon;//아이콘
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Ready;//쿨타임시에만 나오는 회색 막
	weak_ptr<CPlayer_ProgressBar>	m_pPlagueWeapon_Border;//쿨타임 알려주는 게이지 프레임
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Decoration;//쿨타임 알려주는 게이지 프레임


private:
	_float			m_fCoolTime;
	_float			m_fNowTime;

public:
	void Free();

};

END