#pragma once

#include "UI.h"

BEGIN(Client)

class CHUD_PlagueWeaponBase;
class CHUD_PlagueWeapon_Main;
class CHUD_PlagueWeapon_Steal;
class CEasingComponent_Alpha;

class CComponent_Alpha;


class CHUD_PlagueWeapon : public CUI
{
	GAMECLASS_H(CHUD_PlagueWeapon);
	CLONE_H(CHUD_PlagueWeapon, CGameObject);
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) { return S_OK; }

public:
	virtual void		OnLevelEnter() override;

private:
	virtual void SetUp_Component() override; 
	



private:
	weak_ptr<CHUD_PlagueWeapon_Main> m_pMainSkill;

	weak_ptr<CHUD_PlagueWeapon_Main> m_pSubSkill;


	weak_ptr<CHUD_PlagueWeapon_Steal> m_pStealSkill;

private:
	weak_ptr<CEasingComponent_Alpha> m_pEasingAlpha;

};
END

