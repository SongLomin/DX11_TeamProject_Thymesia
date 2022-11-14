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
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Main;//������
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Icon;//������
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Ready;//��Ÿ�ӽÿ��� ������ ȸ�� ��
	weak_ptr<CPlayer_ProgressBar>	m_pPlagueWeapon_Border;//��Ÿ�� �˷��ִ� ������ ������
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Decoration;//��Ÿ�� �˷��ִ� ������ ������


private:
	_float			m_fCoolTime;
	_float			m_fNowTime;

public:
	void Free();

};

END