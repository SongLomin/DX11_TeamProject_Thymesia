#pragma once
#include "HUD_PlagueWeapon.h"

BEGIN(Client)

class CCustomUI;
class CPlayer_ProgressBar;
class CHUD_Hover;
class CHUD_PlagueWeapon_Steal_Icon;

class CHUD_PlagueWeapon_Steal :
	public CHUD_PlagueWeaponBase
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

private:
	weak_ptr<CHUD_PlagueWeapon_Steal_Icon>	m_pPlagueWeapon_Icon;//아이콘
	weak_ptr<CCustomUI>	m_pPlagueWeapon_Decoration;//쿨타임 알려주는 게이지 프레임

public:
	void Free();

};

END