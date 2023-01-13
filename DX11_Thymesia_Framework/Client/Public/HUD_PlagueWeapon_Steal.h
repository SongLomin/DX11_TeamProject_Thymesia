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
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	/*Delegate  From SkillSystem*/
	virtual void		Call_OnChangeSkill(weak_ptr<CSkill_Base> pSkillBase) override;

	/*Delegate From SkillBase*/
	virtual void		Call_UseStartSkill() override;
	virtual void		Call_UpdateCoolDown(_float fCoolDownRatio) override;
	virtual void		Call_EndCoolDown() override;

private:
		virtual void			Bind_Player() override;

private:
	weak_ptr<CCustomUI>			m_pPlagueWeapon_Decoration;	//쿨타임 알려주는 게이지 프레임

public:
	void Free();

};

END