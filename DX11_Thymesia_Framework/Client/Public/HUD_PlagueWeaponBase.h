#pragma once
#include "CustomUI.h"

BEGIN(Client)

class CCustomUI;
class CProgressBar;
class CHUD_Hover;


class CHUD_PlagueWeaponBase :
    public CCustomUI
{
	GAMECLASS_H(CHUD_PlagueWeaponBase);
	CLONE_H(CHUD_PlagueWeaponBase, CGameObject);
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);
public:
	/*Delegate Funtion*/
	virtual void		Call_UseSkill(_float _fCoolTime);
	
public:
	//HoverEvent
	virtual void			Call_HoverFadeEnd(FADER_TYPE eFaderType);
	
public:
	virtual void			Bind_Player();

private:
	


protected:
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Main;//������
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Ready;//��Ÿ�ӽÿ��� ������ ȸ�� ��
	weak_ptr<CProgressBar>	m_pPlagueWeapon_Border;//��Ÿ�� �˷��ִ� ������ ������
	weak_ptr<CHUD_Hover> m_pHover;//��ų �� �� �������� ������ ����Ʈ.

protected:
	Engine::FaderDesc m_tFaderDesc; //For HUD_Hover;

protected:
	_float			m_fCoolTime;
	_float			m_fNowTime;
	_bool			m_bIsCoolTime;
public:
	void Free();

};

END