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
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Main;//������
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Icon;//������
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Ready;//��Ÿ�ӽÿ��� ������ ȸ�� ��
	weak_ptr<CPlayer_ProgressBar>	m_pPlagueWeapon_Border;//��Ÿ�� �˷��ִ� ������ ������
	weak_ptr<CHUD_Hover> m_pHover;//��ų �� �� �������� ������ ����Ʈ.

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