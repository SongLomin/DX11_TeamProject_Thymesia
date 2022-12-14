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
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Main;//프레임
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Ready;//쿨타임시에만 나오는 회색 막
	weak_ptr<CProgressBar>	m_pPlagueWeapon_Border;//쿨타임 알려주는 게이지 프레임
	weak_ptr<CHUD_Hover> m_pHover;//스킬 쿨 다 돌았을시 나오는 이펙트.

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