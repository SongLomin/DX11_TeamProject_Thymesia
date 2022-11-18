#pragma once
#include "HUD_PlagueWeapon.h"


BEGIN(Engine)
class CTexture;
END

BEGIN(Client)

class CCustomUI;
class CHUD_PlagueWeapon_Steal_Icon;
class CHUD_Hover;

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
	weak_ptr<CCustomUI>				m_pPlagueWeapon_Decoration;//��Ÿ�� �˷��ִ� ������ ������
	weak_ptr<CHUD_PlagueWeapon_Steal_Icon>	m_pPlagueWeapon_Icon;//������
public:
	void Free();

};

END