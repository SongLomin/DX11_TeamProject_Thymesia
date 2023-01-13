#pragma once
#include "HUD_PlagueWeaponBase.h"


BEGIN(Client)

class CCustomUI;

class CHUD_PlagueWeapon_Main : public CHUD_PlagueWeaponBase
{
public:
	friend class CHUD_PlagueWeapon;
	GAMECLASS_H(CHUD_PlagueWeapon_Main);
	CLONE_H(CHUD_PlagueWeapon_Main, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);
	virtual void	Set_SocketType(_uint iSocket_Type);


};

END

